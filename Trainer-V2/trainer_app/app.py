from flask import Flask, render_template, jsonify, request
import os
import sys

# Add backend directory to path so we can import modules
sys.path.append(os.path.dirname(__file__))
sys.path.append(os.path.join(os.path.dirname(__file__), 'backend'))

from project_manager import ProjectManager
from backend.ingest import TrainerIngest
# from backend.dsp.horizon_drive import HorizonDrive # Import when needed for training
# from backend.dsp.pv_vitriol import PVVitriolAmp 

app = Flask(__name__)
project_manager = ProjectManager()

# Placeholder for the active training session
active_trainer = None


@app.route('/')
def index():
    return render_template('index.html')

@app.route('/api/scan', methods=['GET'])
def scan_files():
    # Helper to scan directory and return file status
    status = project_manager.scan_project_files()
    return jsonify(status)

import subprocess

@app.route('/api/set_root', methods=['POST'])
def set_root():
    data = request.json
    new_root = data.get('path')
    if new_root:
        full_path = os.path.abspath(os.path.expanduser(new_root.strip()))
        try:
            os.makedirs(full_path, exist_ok=True)
            project_manager.project_root = full_path
            project_manager.save_config()
            return jsonify({"status": "success", "root": project_manager.project_root})
        except Exception as e:
            return jsonify({"error": str(e)}), 400
    return jsonify({"error": "Invalid path"}), 400

@app.route('/api/pick_folder', methods=['GET'])
def pick_folder():
    """Opens a native macOS folder picker."""
    try:
        # Use AppleScript to open a native folder picker
        script = 'POSIX path of (choose folder with prompt "Select Helix Training Folder")'
        result = subprocess.check_output(['osascript', '-e', script], text=True)
        path = result.strip()
        if path:
            return jsonify({"path": path})
        return jsonify({"cancelled": True})
    except Exception as e:
        print(f"Picker error: {e}")
        return jsonify({"error": str(e)}), 500


@app.route('/api/initialize', methods=['POST'])
def initialize_project():
    success = project_manager.initialize_project_structure()
    if success:
        return jsonify({"status": "success", "message": "Project folders created"})
    return jsonify({"error": "Failed to initialize project"}), 400

@app.route('/api/export_logic_script', methods=['GET'])
def export_logic_script():
    """Generates an AppleScript to automate track creation in Logic Pro."""
    try:
        expected = project_manager.get_expected_files()
        # Filter out anything that isn't a wav (though they all should be)
        names = [f["filename"].replace(".wav", "") for f in expected]
        total_tracks = len(names)
        
        # Format names as AppleScript list string
        names_list_str = "{\"" + "\", \"".join(names) + "\"}"
        
        script_content = f'''
-- Neural Calibration Logic Pro Track Builder (REFINED)
-- This script will create and name {total_tracks} tracks for you.

set trackNames to {names_list_str}
set totalCount to {total_tracks}

display dialog "Ready to build " & totalCount & " tracks in Logic Pro?

IMPORTANT: 
1. Logic Pro MUST be the active window.
2. Select an existing track before starting.
3. Do NOT touch your mouse/keyboard during the process." buttons {{"Cancel", "Start"}} default button "Start"

tell application "Logic Pro"
    activate
    delay 1
end tell

set idx to 1
repeat with tName in trackNames
    tell application "System Events"
        tell process "Logic Pro"
            -- 1. Create New Audio Track (Cmd + Opt + A)
            keystroke "a" using {{command down, option down}}
            delay 0.6 -- Wait for track creation
            
            -- 2. Trigger Rename (Shift + Return)
            -- This ensures the focus is definitely on the name field
            keystroke return using {{shift down}}
            delay 0.3
            
            -- 3. Enter the target name
            keystroke tName
            delay 0.2
            keystroke return
            delay 0.4 -- Cooldown before next track
        end tell
    end tell
    
    -- Progress update every 20 tracks
    if (idx mod 20 is 0) then
        display notification "Created " & idx & " of " & totalCount & " tracks..." with title "Logic Builder"
    end if
    set idx to idx + 1
end repeat

display notification "All " & totalCount & " tracks created successfully!" with title "Logic Builder"
display dialog "Build Complete! All " & totalCount & " tracks are ready." buttons {{"Awesome"}} default button "Awesome"
'''
        
        script_path = os.path.join(project_manager.project_root, "Logic_Track_Builder.applescript")
        with open(script_path, "w") as f:
            f.write(script_content)
            
        return jsonify({"status": "success", "path": script_path, "count": total_tracks})
    except Exception as e:
        return jsonify({"error": str(e)}), 500

from flask import Response, stream_with_context
import time

@app.route('/api/train', methods=['POST'])
def train_model():
    try:
        data = request.json
        block_id = data.get('block_id', "01_Horizon_Pedal")
        epochs = int(data.get('epochs', 100))
        chunk_seconds = int(data.get('chunk_seconds', 10))
        
        from trainer_engine import TrainerEngine
        engine = TrainerEngine(project_manager.project_root)
        
        blocks = project_manager.get_blocks_config()
        if block_id not in blocks:
            return jsonify({"error": f"Block {block_id} configuration not found"}), 400
            
        knobs = blocks[block_id].get("knobs", [])
        
        def generate():
            try:
                for message in engine.train_block(block_id, knobs, epochs=epochs, chunk_seconds=chunk_seconds):
                    # Flask streaming requires yielding strings/bytes
                    # We'll use a delimiter so the frontend can split them if they arrive together
                    yield f"{message}\n"
                    # Small sleep to ensure the UI has time to breathe 
                    time.sleep(0.01)
            except Exception as e:
                yield f"ERROR:Training Engine Crash: {str(e)}\n"

        return Response(stream_with_context(generate()), mimetype='text/plain')
    except Exception as e:
        print(f"Training route error: {e}")
        return jsonify({"error": str(e), "status": "failed"}), 500

@app.route('/api/export_config', methods=['GET'])
def export_config():
    """Exports the capture config JSON to the project root."""
    try:
        block_id = request.args.get('block_id', "01_Horizon_Pedal")
        config = project_manager.export_capture_config(block_id)
        if config:
            import json
            filename = f"Capture_Config_{block_id}.json"
            config_path = os.path.join(project_manager.project_root, filename)
            with open(config_path, "w") as f:
                json.dump(config, f, indent=4)
            return jsonify({"status": "success", "path": config_path, "config": config})
        return jsonify({"error": "Block not found"}), 404
    except Exception as e:
        return jsonify({"error": str(e)}), 500

if __name__ == '__main__':
    # Use port 5001 to avoid conflict with MacOS AirPlay Receiver (port 5000)
    app.run(debug=True, port=5001)
