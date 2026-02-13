import torch
import torch.optim as optim
import json
import os
import time
from ingest import TrainerIngest
from dsp.horizon_drive import HorizonDrive
from dsp.pv_vitriol import PVVitriolAmp

class TrainerEngine:
    def __init__(self, project_root):
        self.project_root = project_root
        self.ingest = TrainerIngest()
        # For Recursive DSP (Biquads), CPU is often faster/more stable than MPS
        self.device = torch.device("cpu")
        # Stability fix for Mac mutex crashes during backprop
        torch.set_num_threads(1)
        
    def train_block(self, block_id, knob_targets, epochs=100, chunk_seconds=10):
        """
        Generator that trains the block and yields logs.
        """
        yield f"LOG:Engine initializing on {self.device} (Stable Mode)..."
        
        # 1. Load Reference Input
        input_path = os.path.join(self.project_root, "input.wav")
        if not os.path.exists(input_path):
            yield 'ERROR:Source signal "input.wav" not found in project root.'
            return
            
        import soundfile as sf
        input_audio, sr = sf.read(input_path)
        if input_audio.ndim > 1: input_audio = input_audio[:, 0]
        
        # Calculate chunk size
        if chunk_seconds > 0:
            max_samples = int(sr * chunk_seconds)
            input_tensor_short = torch.from_numpy(input_audio[:max_samples]).float().to(self.device).detach()
            yield f"LOG:Using {chunk_seconds}s snapshots for speed."
        else:
            input_tensor_short = torch.from_numpy(input_audio).float().to(self.device).detach()
            yield "LOG:Using FULL audio (Slow Mode)."
        
        # 2. Select Model
        if block_id == "01_Horizon_Pedal":
            model = HorizonDrive(sample_rate=sr).to(self.device)
        elif block_id == "02_Invective_Amp":
            model = PVVitriolAmp(sample_rate=sr).to(self.device)
        else:
            yield f"ERROR:Unknown block: {block_id}"
            return
            
        # 3. Gather Training Data
        from project_manager import ProjectManager
        pm = ProjectManager(self.project_root)
        all_expected = pm.scan_project_files()["files"]
        block_files = [f for f in all_expected if f["block"] == block_id and f["status"] == "READY"]
        
        if not block_files:
            yield f"ERROR:No READY audio files found for {block_id}."
            return
            
        dataset = []
        for i, bf in enumerate(block_files):
            out_path = os.path.join(self.project_root, bf["rel_path"])
            out_audio, _ = sf.read(out_path)
            if out_audio.ndim > 1: out_audio = out_audio[:, 0]
            
            target_slice = out_audio[:len(input_tensor_short)]
            target_tensor = torch.from_numpy(target_slice).float().to(self.device).detach()
            
            if bf["knob"] == "Chaos":
                settings = bf["chaos_settings"]
            else:
                settings = {k: 5.0 for k in knob_targets}
                if bf["knob"] == "Attack" and "Attack" in settings: settings["Attack"] = 3
                settings[bf["knob"]] = bf["value"]
                
                # Fill in any missing knobs with defaults
                for k in knob_targets:
                    if k not in settings:
                        settings[k] = 3.0 if k == "Attack" else 5.0

            # Convert all setting values to tensors
            knob_tensors = {k: torch.tensor(float(v)).to(self.device).detach() for k, v in settings.items()}
                
            dataset.append({
                "target": target_tensor,
                "knobs": knob_tensors
            })
            
        yield f"LOG:Starting {epochs} epochs of refinement..."

        # 4. Optimization Loop
        optimizer = optim.Adam(model.parameters(), lr=0.01)
        loss_fn = torch.nn.MSELoss()
        start_time = time.time()
        num_files = len(dataset)
        
        for epoch in range(1, epochs + 1):
            total_loss = 0
            optimizer.zero_grad()
            
            for f_idx, item in enumerate(dataset):
                # Forward pass
                pred = model(input_tensor_short, **item["knobs"])
                loss = loss_fn(pred, item["target"])
                
                # Backward pass
                loss.backward()
                total_loss += loss.item()
                
                # Granular sub-progress
                if f_idx % 5 == 0 or f_idx == num_files - 1:
                    yield f"SUBPROGRESS:{f_idx+1}:{num_files}"
                    # Print heartbeat to console
                    print(f"[{block_id}] Epoch {epoch} | Step {f_idx+1}/{num_files}...")
            
            optimizer.step()
            
            # Global progress
            avg_loss = total_loss / num_files
            elapsed = time.time() - start_time
            avg_epoch_time = elapsed / epoch
            remaining = epochs - epoch
            eta_seconds = int(avg_epoch_time * remaining)
            eta_str = f"{eta_seconds // 60}m {eta_seconds % 60}s" if eta_seconds > 60 else f"{eta_seconds}s"

            yield f"PROGRESS:{epoch}:{epochs}:{eta_str}"
            yield f"LOG:Epoch {epoch} complete | Loss: {avg_loss:.8f} | ETA: {eta_str}"
            print(f"[{block_id}] Epoch {epoch} DONE. Loss: {avg_loss:.8f}")
            
        # 5. Export Weights
        yield "LOG:Training Complete. Exporting neural weights..."
        weights = model.export_weights()
        
        # Ensure 'models' directory exists
        models_dir = os.path.join(self.project_root, "models")
        if not os.path.exists(models_dir):
            os.makedirs(models_dir)
            
        # Detect next version number (v001, v002...)
        existing_models = [f for f in os.listdir(models_dir) if f.startswith("v") and f.endswith(".json")]
        version_num = 1
        if existing_models:
            try:
                versions = [int(f.split('_')[0][1:]) for f in existing_models if f.split('_')[0][1:].isdigit()]
                if versions: version_num = max(versions) + 1
            except: pass
            
        v_str = f"v{version_num:03d}"
        loss_fixed = f"{avg_loss:.6f}".replace(".", "p")
        filename = f"{v_str}_{block_id}_Epochs{epochs}_Chunk{chunk_seconds}s_Loss{loss_fixed}.json"
        export_path = os.path.join(models_dir, filename)

        with open(export_path, "w") as f:
            json.dump(weights, f, indent=4)
            
        total_elapsed = int(time.time() - start_time)
        yield f"DONE:{json.dumps({'status': 'success', 'message': f'Trained {block_id} in {total_elapsed}s', 'path': f'models/{filename}'})}"
