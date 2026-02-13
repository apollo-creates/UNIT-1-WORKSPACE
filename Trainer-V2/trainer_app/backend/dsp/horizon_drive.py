import torch
import torch.nn as nn
import torchaudio.functional as F
from backend.dsp.base_dsp import BaseDSP

class HorizonDrive(BaseDSP):
    """
    Differentiable model of the Horizon Devices Precision Drive.
    Topology: Input -> Variable HPF (Attack) -> Gain Stage (Drive) -> Hard Clipper -> Tone Filter (Bright) -> Output Level
    """
    def __init__(self, sample_rate=48000):
        super().__init__()
        self.sample_rate = sample_rate
        
        # Learnable Mappings (Knob -> DSP)
        # Drive: gain = a * (knob^b) + c
        self.drive_a = nn.Parameter(torch.tensor(1.0))
        self.drive_b = nn.Parameter(torch.tensor(2.0))
        self.drive_c = nn.Parameter(torch.tensor(1.0))
        
        # Attack: cutoff = a * knob + b (for 6 steps)
        self.attack_a = nn.Parameter(torch.tensor(180.0))
        self.attack_b = nn.Parameter(torch.tensor(100.0))
        
        # Bright: cutoff = a * knob + b
        self.bright_a = nn.Parameter(torch.tensor(950.0))
        self.bright_b = nn.Parameter(torch.tensor(500.0))
        
        # Level (Global Scaler)
        self.level_scaler = nn.Parameter(torch.tensor(0.2))

    def forward(self, x, **knobs):
        """
        x: Input audio tensor (batch, time)
        """
        drive_knob = knobs.get("Drive", torch.tensor(5.0))
        attack_knob = knobs.get("Attack", torch.tensor(3.0))
        bright_knob = knobs.get("Bright", torch.tensor(5.0))

        # 1. Variable HPF (Attack)
        attack_cutoff = attack_knob * self.attack_a + self.attack_b
        # Clamp cutoff to safe range [20, 20000]
        attack_cutoff = torch.clamp(attack_cutoff, 20.0, 20000.0)
        x_hpf = F.highpass_biquad(x, self.sample_rate, attack_cutoff, Q=0.707)
        
        # 2. Gain Stage (Drive)
        gain_val = self.drive_c + self.drive_a * torch.pow(drive_knob, self.drive_b)
        x_driven = x_hpf * gain_val
        
        # 3. Hard Clipper
        x_clipped = torch.clamp(x_driven, -1.0, 1.0)
        
        # 4. Tone Filter (Bright)
        bright_cutoff = bright_knob * self.bright_a + self.bright_b
        bright_cutoff = torch.clamp(bright_cutoff, 20.0, 20000.0)
        x_tone = F.lowpass_biquad(x_clipped, self.sample_rate, bright_cutoff, Q=0.707)
        
        # 5. Output level
        output = x_tone * self.level_scaler
        
        return output

    def export_weights(self):
        return {
            "drive": {"a": self.drive_a.item(), "b": self.drive_b.item(), "c": self.drive_c.item()},
            "attack": {"a": self.attack_a.item(), "b": self.attack_b.item()},
            "bright": {"a": self.bright_a.item(), "b": self.bright_b.item()},
            "level": self.level_scaler.item()
        }
