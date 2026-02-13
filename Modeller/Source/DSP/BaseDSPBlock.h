#pragma once

#include <juce_audio_basics/juce_audio_basics.h>

class BaseDSPBlock {
public:
  virtual ~BaseDSPBlock() = default;

  virtual void prepare(double sampleRate, int samplesPerBlock) = 0;

  virtual void process(juce::AudioBuffer<float> &buffer) {
    if (bypassed)
      return;

    for (int channel = 0; channel < buffer.getNumChannels(); ++channel) {
      auto *p = buffer.getWritePointer(channel);
      for (int i = 0; i < buffer.getNumSamples(); ++i) {
        p[i] = processSample(p[i], channel);
      }
    }
  }

  virtual float processSample(float input, int channel) = 0;

  void setBypassed(bool b) { bypassed = b; }
  bool isBypassed() const { return bypassed; }

protected:
  double sampleRate = 44100.0;
  bool bypassed = false;
};
