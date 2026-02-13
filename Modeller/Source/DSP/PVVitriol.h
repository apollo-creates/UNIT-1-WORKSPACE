#pragma once

#include "BaseDSPBlock.h"
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

    updateFilters();
  }

  float processSample(float x, int /*channel*/) override {
    // 1. Pre-Gain (Input Drive)
    x *= juce::Decibels::decibelsToGain(gain * 60.0f);

    // 2. Waveshaper (The "Neural" Part)
    x = std::tanh(x * distortionScale);

    // 3. Tonestack
    x = lowFilter.processSample(x);
    x = midFilter.processSample(x);
    x = highFilter.processSample(x);

    // 4. Output Level
    x *= outputLevel;

    return x;
  }

  void setGain(float g) { gain = g / 10.0f; }
  void setLow(float l) {
    lowVal = l;
    updateFilters();
  }
  void setMid(float m) {
    midVal = m;
    updateFilters();
  }
  void setHigh(float h) {
    highVal = h;
    updateFilters();
  }
  void setLevel(float l) { outputLevel = l / 10.0f; }

private:
  void updateFilters() {
    if (sampleRate <= 0.0)
      return;

    // Low Shelf around 100Hz
    lowFilter.coefficients = juce::dsp::IIR::Coefficients<float>::makeLowShelf(
        sampleRate, 100.0f, 0.707f,
        juce::Decibels::decibelsToGain((lowVal - 5.0f) * 2.0f));

    // Mid Peak around 700Hz
    midFilter.coefficients =
        juce::dsp::IIR::Coefficients<float>::makePeakFilter(
            sampleRate, 700.0f, 1.0f,
            juce::Decibels::decibelsToGain((midVal - 5.0f) * 3.0f));

    // High Shelf around 4kHz
    highFilter.coefficients =
        juce::dsp::IIR::Coefficients<float>::makeHighShelf(
            sampleRate, 4000.0f, 0.707f,
            juce::Decibels::decibelsToGain((highVal - 5.0f) * 2.0f));
  }

  juce::dsp::IIR::Filter<float> lowFilter;
  juce::dsp::IIR::Filter<float> midFilter;
  juce::dsp::IIR::Filter<float> highFilter;

  float gain = 0.5f;
  float lowVal = 5.0f;
  float midVal = 5.0f;
  float highVal = 5.0f;
  float outputLevel = 0.8f;
  float distortionScale = 1.0f;
};
