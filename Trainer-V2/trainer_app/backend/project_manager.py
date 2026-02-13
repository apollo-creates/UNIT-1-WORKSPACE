import os
import glob

class ProjectManager:
    def __init__(self, project_root=None):
        self.config_file = os.path.expanduser("~/.helix_trainer_config")
        if project_root is None:
            if os.path.exists(self.config_file):
                with open(self.config_file, "r") as f:
                    project_root = f.read().strip()
            
            if not project_root:
                project_root = os.environ.get("TRAINER_ROOT", "~/Music/Helix_Training")
        
        self.project_root = os.path.abspath(os.path.expanduser(project_root))
        self.save_config()

    def save_config(self):
        try:
            with open(self.config_file, "w") as f:
                f.write(self.project_root)
        except:
            pass

    def get_blocks_config(self):
        return {
            "01_Horizon_Pedal": {
                "knobs": ["Drive", "Attack", "Bright"],
                "sweeps": {
                    "Drive": [0.0, 2.5, 5.0, 7.5, 10.0],
                    "Attack": [1, 2, 3, 4, 5, 6],
                    "Bright": [0.0, 2.5, 5.0, 7.5, 10.0]
                },
                "chaos_count": 30,
                "chaos_knobs": ["Drive", "Attack", "Bright"],
                "subfolder": "01_Single_Sweeps",
                "chaos_subfolder": "02_Chaos_Mode"
            },
            "02_Invective_Amp": {
                "knobs": ["Pre_Gain", "Bass", "Mid", "Treble", "Post_Gain", "Master", "Presence", "Depth", "Sag", "Bias", "Bias_X"],
                "sweeps": {
                    "dataset": [0.0, 2.5, 5.0, 7.5, 10.0]
                },
                "chaos_count": 50,
                "chaos_knobs": ["Pre_Gain", "Bass", "Mid", "Treble", "Post_Gain", "Master", "Presence", "Depth", "Sag", "Bias", "Bias_X"],
                "subfolder": "01_Single_Sweeps",
                "chaos_subfolder": "02_Chaos_Mode"
            }
        }
        
    def get_expected_files(self):
        """Generates a list of all expected files based on the blocks configuration."""
        files = []
        blocks = self.get_blocks_config()
        for block_name, block_data in blocks.items():
            # 1. Single Sweeps
            subfolder = block_data.get("subfolder", "")
            for knob in block_data.get("knobs", []):
                if knob in block_data.get("sweeps", {}):
                    values = block_data["sweeps"][knob]
                else:
                    values = block_data.get("sweeps", {}).get("dataset", [])
                
                for val in values:
                    # Format value string
                    if isinstance(val, int):
                        val_str = str(val)
                    else:
                        val_str = str(val).replace('.', 'p')
                    
                    # Ensure filename uses underscores but knob name stays for UI
                    filename_knob = knob.replace(' ', '_')
                    filename = f"{block_name}_{filename_knob}_{val_str}.wav"
                    
                    rel_path = os.path.join(block_name, subfolder, filename)
                    
                    files.append({
                        "block": block_name,
                        "knob": knob,
                        "value": val,
                        "filename": filename,
                        "rel_path": rel_path
                    })
            
            # 2. Chaos Mode Sweeps
            chaos_count = block_data.get("chaos_count", 0)
            if chaos_count > 0:
                import random
                chaos_knobs = block_data.get("chaos_knobs", block_data.get("knobs", []))
                chaos_subfolder = block_data.get("chaos_subfolder", "")
                
                for i in range(1, chaos_count + 1):
                    # Deterministic random values based on index
                    rng = random.Random(f"{block_name}_chaos_{i}")
                    
                    settings = {}
                    for p in chaos_knobs:
                        if p == "Attack":
                            # 6-step switch (1-6)
                            settings[p] = rng.randint(1, 6)
                        else:
                            # Standard knob (0-10)
                            settings[p] = round(rng.uniform(0.0, 10.0), 1)
                    
                    # Create a summary string for the UI with specific labels
                    label_map = {
                        "Pre_Gain": "Pre", "Post_Gain": "Post", "Presence": "Pres",
                        "Bias_X": "Bias X", "Bias": "Bias", "Attack": "Atk",
                        "Drive": "Driv", "Bright": "Bri", "Master": "Mast",
                        "Treble": "Treb", "Bass": "Bass", "Mid": "Mid",
                        "Depth": "Dept", "Sag": "Sag"
                    }
                    
                    settings_str = ", ".join([f"{label_map.get(k, k[:4])}:{v}" for k, v in settings.items()])
                    
                    # Gemini spec filename: Chaos_Full_01.wav
                    filename = f"Chaos_Full_{i:02d}.wav"
                    rel_path = os.path.join(block_name, chaos_subfolder, filename)
                    
                    files.append({
                        "block": block_name,
                        "knob": "Chaos",
                        "value": i,
                        "filename": filename,
                        "rel_path": rel_path,
                        "chaos_settings": settings,
                        "chaos_summary": settings_str
                    })
        return files

    def initialize_project_structure(self):
        """Creates the entire directory structure for the project and generates chaos maps."""
        if not self.project_root:
            return False
            
        blocks = self.get_blocks_config()
        expected_files = self.get_expected_files()
        
        for block_name, block_data in blocks.items():
            # Create Block Folder
            block_path = os.path.join(self.project_root, block_name)
            os.makedirs(block_path, exist_ok=True)
            
            # Create Single Sweeps Folder
            if "subfolder" in block_data:
                os.makedirs(os.path.join(block_path, block_data["subfolder"]), exist_ok=True)
                
            # Create Chaos Folder
            if "chaos_subfolder" in block_data:
                chaos_path = os.path.join(block_path, block_data["chaos_subfolder"])
                os.makedirs(chaos_path, exist_ok=True)
                
                # Generate chaos_map.csv
                chaos_knobs = block_data.get("chaos_knobs", [])
                if chaos_knobs:
                    map_file = os.path.join(chaos_path, "chaos_map.csv")
                    with open(map_file, "w") as f:
                        header = "Filename," + ",".join(chaos_knobs)
                        f.write(header + "\n")
                        
                        # Find all chaos files for this block
                        chaos_files = [ef for ef in expected_files if ef["block"] == block_name and ef["knob"] == "Chaos"]
                        for cf in chaos_files:
                            settings = cf["chaos_settings"]
                            row = [cf["filename"]] + [str(settings[p]) for p in chaos_knobs]
                            f.write(",".join(row) + "\n")
        return True

    def scan_project_files(self):
        """Checks which files exist in the project directory."""
        if not self.project_root or not os.path.exists(self.project_root):
            return {"error": "Project directory not found", "files": [], "root": self.project_root, "input_exists": False}
            
        input_exists = os.path.exists(os.path.join(self.project_root, "input.wav"))
        expected = self.get_expected_files()
        results = []
        
        for item in expected:
            full_path = os.path.join(self.project_root, item["rel_path"])
            exists = os.path.exists(full_path)
            results.append({
                **item,
                "status": "READY" if exists else "MISSING",
                "full_path": full_path
            })
            
        return {"files": results, "root": self.project_root, "input_exists": input_exists}
    def export_capture_config(self, block_name):
        """Generates a JSON configuration for the Headless Capture app."""
        blocks = self.get_blocks_config()
        if block_name not in blocks:
            return None
            
        data = blocks[block_name]
        expected_files = self.get_expected_files()
        
        # Filter files for ONLY this block
        block_files = [f for f in expected_files if f["block"] == block_name]
        
        config = {
            "project_name": block_name,
            "project_root": self.project_root,
            "type": "AMP" if "Amp" in block_name else "PEDAL",
            "knobs": [],
            "passes": []
        }
        
        # 1. Export Knob Definitions (for UI setup)
        # Use chaos_knobs if they exist as they are the superset
        display_knobs = data.get("chaos_knobs", data.get("knobs", []))
        for knob in display_knobs:
            sweep_vals = data.get("sweeps", {}).get(knob, data.get("sweeps", {}).get("dataset", [0.0, 10.0]))
            min_v = min(sweep_vals) / 10.0 # Normalize to 0-1
            max_v = max(sweep_vals) / 10.0
            config["knobs"].append({
                "id": knob,
                "name": knob,
                "min": min_v,
                "max": max_v,
                "type": "Stepped" if knob == "Attack" else "Linear"
            })
            
        # 2. Export All Passes (The actual roadmap for the capture engine)
        for f in block_files:
            target_dir = os.path.join(block_name, data.get("subfolder", "01_Single_Sweeps"))
            if f["knob"] == "Chaos":
                target_dir = os.path.join(block_name, data.get("chaos_subfolder", "02_Chaos_Mode"))
            
            pass_data = {
                "filename": f["filename"],
                "target_dir": target_dir,
                "settings": {}
            }
            
            if f["knob"] == "Chaos":
                # Multiple knobs at once
                for k, v in f["chaos_settings"].items():
                    pass_data["settings"][k] = v / 10.0 # Export as 0-1 for JUCE
            else:
                # Single knob sweep (others at default 0.5/5.0)
                # First, ensure ALL knobs are in settings at mid value
                for k in display_knobs:
                    pass_data["settings"][k] = 0.5
                # Then set the specific sweep value
                pass_data["settings"][f["knob"]] = f["value"] / 10.0
                
            config["passes"].append(pass_data)
            
        return config
