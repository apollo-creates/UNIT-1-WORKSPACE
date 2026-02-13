#pragma once

#include "BaseDSPBlock.h"
#include <cmath>
#include <juce_dsp/juce_dsp.h>

class HorizonDrive : public BaseDSPBlock {
public:
  HorizonDrive() {}

  void prepare(double sr, int samplesPerBlock) override {
    sampleRate = sr;
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sr;
    spec.maximumBlockSize = (juce::uint32)samplesPerBlock;
    spec.numChannels = 2;

    attackFilter.prepare(spec);
    brightFilter.prepare(spec);

    updateFilters();
  }

  float processSample(float x, int /*channel*/) override {
    // 1. Variable HPF (Attack)
    x = attackFilter.processSample(x);

    // 2. Gain Stage (Neural Drive Mapping)
    // gain_val = drive_c + drive_a * pow(knobDrive, drive_b)
    float gain_val = drive_c + drive_a * std::pow(knobDrive, drive_b);
    x *= gain_val;

    // 3. Hard Clipping
    x = juce::jlimit(-1.0f, 1.0f, x);

    // 4. Tone Filter (Bright)
    x = brightFilter.processSample(x);

    // 5. Output level
    return x * level_scaler;
  }

  // UI Knob setters
  void setDrive(float d) { knobDrive = d; }
  void setAttack(float a) {
    knobAttack = a;
    updateFilters();
  }
  void setBright(float b) {
    knobBright = b;
    updateFilters();
  }
  void setLevel(float l) { knobLevel = l; }

  // Neural weight setters (matching trainer)
  void set_drive_a(float v) { drive_a = v; }
  void set_drive_b(float v) { drive_b = v; }
  void set_drive_c(float v) { drive_c = v; }
  void set_attack_a(float v) {
    attack_a = v;
    updateFilters();
  }
  void set_attack_b(float v) {
    attack_b = v;
    updateFilters();
  }
  void set_bright_a(float v) {
    bright_a = v;
    updateFilters();
  }
  void set_bright_b(float v) {
    bright_b = v;
    updateFilters();
  }
  void set_level_scaler(float v) { level_scaler = v; }

private:
  void updateFilters() {
    if (sampleRate <= 0.0)
      return;

    // Filter cutoffs based on trained linear mappings
    float attack_cutoff = (knobAttack * attack_a) + attack_b;
    attack_cutoff = juce::jlimit(20.0f, 20000.0f, attack_cutoff);
    attackFilter.coefficients =
        juce::dsp::IIR::Coefficients<float>::makeHighPass(sampleRate,
                                                          attack_cutoff);

    float bright_cutoff = (knobBright * bright_a) + bright_b;
    bright_cutoff = juce::jlimit(20.0f, 20000.0f, bright_cutoff);
    brightFilter.coefficients =
        juce::dsp::IIR::Coefficients<float>::makeLowPass(sampleRate,
                                                         bright_cutoff);
  }

  juce::dsp::IIR::Filter<float> attackFilter, brightFilter;

  float knobDrive = 5.0f, knobAttack = 3.0f, knobBright = 5.0f,
        knobLevel = 1.0f;

  float drive_a = 0.75f, drive_b = 1.2f, drive_c = 4.3f;
  float attack_a = 180.0f, attack_b = 100.0f;
  float bright_a = 950.0f, bright_b = 500.0f;
  float level_scaler = 1.0f;
};
