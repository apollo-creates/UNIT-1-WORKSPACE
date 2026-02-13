#pragma once

#include "BaseDSPBlock.h"
#include <cmath>
#include <juce_dsp/juce_dsp.h>

class PVVitriolAmp : public BaseDSPBlock {
public:
  PVVitriolAmp() {}

  void prepare(double sr, int samplesPerBlock) override {
    sampleRate = sr;

    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sr;
    spec.maximumBlockSize = (juce::uint32)samplesPerBlock;
    spec.numChannels = 2;

    lowFilter.prepare(spec);
    midFilter.prepare(spec);
    highFilter.prepare(spec);
    presenceFilter.prepare(spec);
    depthFilter.prepare(spec);

    updateFilters();
  }

  float processSample(float x, int /*channel*/) override {
    // 1. Pre-Gain & Preamp Distortion (matching pv_vitriol.py)
    float gain_val = preg_b + (knobPreGain * preg_a);
    x *= gain_val;

    // Soft clipping tube emulation
    x = std::tanh(x);

    // 2. Parallel Tone Stack
    float x_bass = lowFilter.processSample(x) * (knobBass * bass_a / 5.0f);
    float x_mid = midFilter.processSample(x) * (knobMid * mid_a / 5.0f);
    float x_treb = highFilter.processSample(x) * (knobTreb * treb_a / 5.0f);

    // post_gain from python: x_eq = (x_bass + x_mid + x_treb) * (1.0 +
    // post_gain * 0.2)
    float x_eq = (x_bass + x_mid + x_treb) * (1.0f + knobPostGain * 0.2f);

    // 3. Power Amp Simulation (with Sag / Bias / Bias X)
    // Bias offset
    float bias_offset = (knobBias - 5.0f) * bias_a;
    float x_biased = x_eq + bias_offset;

    // Dynamic Sag (Simplified envelope like python)
    // self.sag_a * env
    // We'll use a local peak follower or just absolute value for simplicity to
    // match trainer's mean() logic
    float sag_factor = 1.0f - (knobSag * sag_a * std::abs(x_eq));

    // Bias X - extra non-linearity/offset for power amp
    float x_power = x_biased * (knobMaster * master_a) * sag_factor +
                    (knobBiasX - 5.0f) * 0.01f;
    float x_power_sat = std::tanh(x_power);

    // 4. Presence / Depth (Negative Feedback)
    float x_pres = presenceFilter.processSample(x_power_sat) *
                   (knobPresence * pres_a / 5.0f);
    float x_res =
        depthFilter.processSample(x_power_sat) * (knobDepth * depth_a / 5.0f);

    float output =
        (x_power_sat + (x_pres * 0.2f) + (x_res * 0.2f)) * level_scaler;

    return output;
  }

  // UI Knobs
  void setPreGain(float v) { knobPreGain = v; }
  void setBass(float v) { knobBass = v; }
  void setMid(float v) { knobMid = v; }
  void setTreb(float v) { knobTreb = v; }
  void setPostGain(float v) { knobPostGain = v; }
  void setMaster(float v) { knobMaster = v; }
  void setPresence(float v) { knobPresence = v; }
  void setDepth(float v) { knobDepth = v; }
  void setSag(float v) { knobSag = v; }
  void setBias(float v) { knobBias = v; }
  void setBiasX(float v) { knobBiasX = v; }

  // Neural Weights (Mapped from JSON)
  void set_preg_a(float v) { preg_a = v; }
  void set_preg_b(float v) { preg_b = v; }
  void set_bass_a(float v) { bass_a = v; }
  void set_mid_a(float v) { mid_a = v; }
  void set_treb_a(float v) { treb_a = v; }
  void set_master_a(float v) { master_a = v; }
  void set_pres_a(float v) { pres_a = v; }
  void set_depth_a(float v) { depth_a = v; }
  void set_sag_a(float v) { sag_a = v; }
  void set_bias_a(float v) { bias_a = v; }
  void set_level_scaler(float v) { level_scaler = v; }

private:
  void updateFilters() {
    if (sampleRate <= 0.0)
      return;
    // Matching frequencies in python trainer
    lowFilter.coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(
        sampleRate, 100.0f, 0.707f);
    midFilter.coefficients = juce::dsp::IIR::Coefficients<float>::makeBandPass(
        sampleRate, 800.0f, 1.0f);
    highFilter.coefficients = juce::dsp::IIR::Coefficients<float>::makeHighPass(
        sampleRate, 3000.0f, 0.707f);
    presenceFilter.coefficients =
        juce::dsp::IIR::Coefficients<float>::makeHighPass(sampleRate, 5000.0f,
                                                          0.707f);
    depthFilter.coefficients = juce::dsp::IIR::Coefficients<float>::makeLowPass(
        sampleRate, 80.0f, 0.707f);
  }

  juce::dsp::IIR::Filter<float> lowFilter, midFilter, highFilter;
  juce::dsp::IIR::Filter<float> presenceFilter, depthFilter;

  // Knob Values (0-10)
  float knobPreGain = 5.0f, knobBass = 5.0f, knobMid = 5.0f, knobTreb = 5.0f;
  float knobPostGain = 5.0f, knobMaster = 5.0f, knobPresence = 5.0f,
        knobDepth = 5.0f;
  float knobSag = 5.0f, knobBias = 5.0f, knobBiasX = 5.0f;

  // Neural Mappings
  float preg_a = 5.0f, preg_b = 1.0f;
  float bass_a = 1.0f, mid_a = 1.0f, treb_a = 1.0f;
  float master_a = 0.5f, pres_a = 1.0f, depth_a = 1.0f;
  float sag_a = 0.1f, bias_a = 0.0f, level_scaler = 0.1f;
};
