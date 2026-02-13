#pragma once

#include "BaseDSPBlock.h"
#include <juce_dsp/juce_dsp.h>

class HorizonDrive : public BaseDSPBlock {
public:
  HorizonDrive() {}

  void prepare(double sr, int samplesPerBlock) override {
    sampleRate = sr;

    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sr;
    spec.maximumBlockSize = (juce::uint32)samplesPerBlock;
    spec.numChannels = 2; // Stereo support

    hpf.prepare(spec);
    toneFilter.prepare(spec);

    updateFilters();
  }

  float processSample(float x, int /*channel*/) override {
    // 1. HPF (Attack)
    x = hpf.processSample(x);

    // 2. Gain Stage
    x *= juce::Decibels::decibelsToGain(drive * 40.0f); // Map 0-10 to 0-40dB

    // 3. Hard Clipping
    x = juce::jlimit(-1.0f, 1.0f, x);

    // 4. Tone Filter (Bright)
    x = toneFilter.processSample(x);

    // 5. Output Level
    x *= level;

    return x;
  }

  void setDrive(float d) { drive = d / 10.0f; }
  void setAttack(float a) {
    attack = a;
    updateFilters();
  }
  void setBright(float b) {
    bright = b;
    updateFilters();
  }
  void setLevel(float l) { level = l / 10.0f; }

private:
  void updateFilters() {
    if (sampleRate <= 0.0)
      return;

    // Typical Attack range 100Hz - 1000Hz
    float hpfFreq = 100.0f + (attack * 90.0f);
    hpf.coefficients =
        juce::dsp::IIR::Coefficients<float>::makeHighPass(sampleRate, hpfFreq);

    // Typical Bright range 1kHz - 10kHz
    float lpFreq = 1000.0f + (bright * 900.0f);
    toneFilter.coefficients =
        juce::dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, lpFreq);
  }

  juce::dsp::IIR::Filter<float> hpf;
  juce::dsp::IIR::Filter<float> toneFilter;

  float drive = 0.5f;
  float attack = 3.0f;
  float bright = 5.0f;
  float level = 1.0f;
};
