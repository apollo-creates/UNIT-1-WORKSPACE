try:
    import torch
except ImportError:
    torch = None

import soundfile as sf
import os
import numpy as np

class TrainerIngest:
    def __init__(self, sample_rate=48000, chunk_duration_sec=10):
        self.sr = sample_rate
        self.chunk_samples = sample_rate * chunk_duration_sec

    def load_sweep_file(self, filepath, knob_values):
        """
        Slices a long sweep file into labeled training pairs.
        
        filepath: Path to your .wav export from Logic
        knob_values: List of the actual knob settings you used (e.g. [0.0, 2.5, 5.0...])
        """
        # 1. Load the big wav file
        try:
            audio, sr = sf.read(filepath)
        except Exception as e:
            print(f"Error reading file {filepath}: {e}")
            return []
        
        # Ensure mono for simplicity (if stereo, take left channel)
        if audio.ndim > 1:
            audio = audio[:, 0]
            
        if torch is None:
            print("PyTorch is not installed. Returning raw numpy array.")
            # Mock behavior or use numpy
            audio_tensor = audio # Keep as numpy
        else:
            # Convert to PyTorch Tensor
            audio_tensor = torch.from_numpy(audio).float()

        
        training_data = []
        
        # 2. Slice it up!
        for i, val in enumerate(knob_values):
            start_sample = i * self.chunk_samples
            end_sample = start_sample + self.chunk_samples
            
            # Safety check to not read past end of file
            if end_sample > len(audio_tensor):
                break
                
            # Extract the 10-second chunk
            chunk = audio_tensor[start_sample:end_sample]
            
            # Store it as a pair: (Audio_Chunk, Knob_Value_Label)
            training_data.append({
                "audio": chunk,
                "label": val,
                "name": f"Knob_Pos_{val}"
            })
            
            # print(f"--> Extracted chunk for Knob Value: {val} (Samples {start_sample} to {end_sample})")
            
        return training_data
