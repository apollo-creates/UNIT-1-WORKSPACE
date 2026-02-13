import torch
import torch.nn as nn
import torchaudio.functional as F
from backend.dsp.base_dsp import BaseDSP

class PVVitriolAmp(BaseDSP):
    """
    Differentiable model of the Peavey Invective (6505-based) Amp.
    Topology: Input -> Pre-Gain -> Waveshaper (Tube Stages) -> Tone Stack (B/M/T) -> Power Amp Simulation (Master/Sag) -> Output
    """
    def __init__(self, sample_rate=48000):
        super().__init__()
        self.sample_rate = sample_rate
        
        # Learnable Mappings (Knob -> DSP)
        # Pre-Gain mapping
        self.preg_a = nn.Parameter(torch.tensor(5.0))
        self.preg_b = nn.Parameter(torch.tensor(1.0))
        
        # Tone Stack mappings
        self.bass_a = nn.Parameter(torch.tensor(1.0))
        self.mid_a = nn.Parameter(torch.tensor(1.0))
        self.treb_a = nn.Parameter(torch.tensor(1.0))
        
        # Power Amp mappings
        self.master_a = nn.Parameter(torch.tensor(0.5))
        self.pres_a = nn.Parameter(torch.tensor(1.0))
        self.depth_a = nn.Parameter(torch.tensor(1.0))
        
        # New: Sag and Bias mappings
        self.sag_a = nn.Parameter(torch.tensor(0.1))
        self.bias_a = nn.Parameter(torch.tensor(0.0))
        
        # Global Level
        self.level_scaler = nn.Parameter(torch.tensor(0.1))

    def forward(self, x, **knobs):
        """
        x: Input audio tensor
        knobs: Normalized 0-10 tensors
        """
        pre_gain = knobs.get("Pre_Gain", torch.tensor(5.0))
        bass = knobs.get("Bass", torch.tensor(5.0))
        mid = knobs.get("Mid", torch.tensor(5.0))
        treble = knobs.get("Treble", torch.tensor(5.0))
        post_gain = knobs.get("Post_Gain", torch.tensor(5.0))
        master = knobs.get("Master", torch.tensor(5.0))
        presence = knobs.get("Presence", torch.tensor(5.0))
        depth = knobs.get("Depth", torch.tensor(5.0))
        sag = knobs.get("Sag", torch.tensor(5.0))
        bias = knobs.get("Bias", torch.tensor(5.0))
        bias_x = knobs.get("Bias_X", torch.tensor(5.0))
        
        # 1. Pre-Gain & Tube Saturation (Preamp)
        gain_val = self.preg_b + (pre_gain * self.preg_a)
        x_pre = x * gain_val
        x_sat = torch.tanh(x_pre) # Soft clipping tube emulation
        
        # 2. Tone Stack (Passive EQ interaction)
        x_bass = F.lowpass_biquad(x_sat, self.sample_rate, 100.0, Q=0.707) * (bass * self.bass_a / 5.0) 
        x_mid = F.bandpass_biquad(x_sat, self.sample_rate, 800.0, Q=1.0) * (mid * self.mid_a / 5.0)
        x_treble = F.highpass_biquad(x_sat, self.sample_rate, 3000.0, Q=0.707) * (treble * self.treb_a / 5.0)
        
        x_eq = (x_bass + x_mid + x_treble) * (1.0 + post_gain * 0.2)
        
        # 3. Power Amp Simulation (with Sag / Bias)
        # Apply Bias (DC offset before saturation)
        bias_offset = (bias - 5.0) * self.bias_a
        x_biased = x_eq + bias_offset
        
        power_drive = master * self.master_a
        
        # Dynamic Sag (Envelope-based compression)
        # Simplified: Master + High Input levels = more sag (squish)
        env = torch.abs(x_eq).mean()
        sag_factor = 1.0 - (sag * self.sag_a * env)
        
        x_power = x_biased * power_drive * sag_factor
        x_power_sat = torch.tanh(x_power) # Power tube saturation
        
        # 4. Presence / Depth (Negative Feedback)
        x_pres = F.highpass_biquad(x_power_sat, self.sample_rate, 5000.0, Q=0.707) * (presence * self.pres_a / 5.0)
        x_res = F.lowpass_biquad(x_power_sat, self.sample_rate, 80.0, Q=0.707) * (depth * self.depth_a / 5.0)
        
        output = (x_power_sat + (x_pres * 0.2) + (x_res * 0.2)) * self.level_scaler
        
        return output

    def export_weights(self):
        return {
            "pre_gain": {"a": self.preg_a.item(), "b": self.preg_b.item()},
            "tone": {"bass": self.bass_a.item(), "mid": self.mid_a.item(), "treb": self.treb_a.item()},
            "power": {
                "master": self.master_a.item(), 
                "presence": self.pres_a.item(), 
                "depth": self.depth_a.item(),
                "sag": self.sag_a.item(),
                "bias": self.bias_a.item()
            },
            "level": self.level_scaler.item()
        }
