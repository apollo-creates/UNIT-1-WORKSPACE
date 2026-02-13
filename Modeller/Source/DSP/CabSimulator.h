#pragma once

#include "BaseDSPBlock.h"
#include <juce_dsp/juce_dsp.h>

class CabSimulator : public BaseDSPBlock {
public:
  CabSimulator() { bypassed = true; }

  void prepare(double sr, int samplesPerBlock) override {
    sampleRate = sr;

    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sr;
    spec.maximumBlockSize = (juce::uint32)samplesPerBlock;
    spec.numChannels = 2;

    convolutionA.prepare(spec);
    convolutionB.prepare(spec);

    tempBuffer.setSize(2, samplesPerBlock);
  }

  void process(juce::AudioBuffer<float> &buffer) override {
    if (bypassed)
      return;

    // If mix is 0.0, only A. If 1.0, only B.
    if (mix <= 0.05f && irALoaded) {
      processConvolution(convolutionA, buffer);
    } else if (mix >= 0.95f && irBLoaded) {
      processConvolution(convolutionB, buffer);
    } else if (irALoaded && irBLoaded) {
      // Mix both
      tempBuffer.makeCopyOf(buffer);
      processConvolution(convolutionA, buffer);     // Result A in buffer
      processConvolution(convolutionB, tempBuffer); // Result B in tempBuffer

      // Linear mix
      for (int channel = 0; channel < buffer.getNumChannels(); ++channel) {
        buffer.addFromWithRamp(channel, 0, tempBuffer.getReadPointer(channel),
                               buffer.getNumSamples(), mix, mix);
        buffer.applyGain(channel, 0, buffer.getNumSamples(), 1.0f - mix);
      }
    } else if (irALoaded) {
      processConvolution(convolutionA, buffer);
    } else if (irBLoaded) {
      processConvolution(convolutionB, buffer);
    }
  }

  float processSample(float input, int /*channel*/) override { return input; }

  bool loadIRA(const juce::File &file) {
    if (!file.existsAsFile())
      return false;
    convolutionA.loadImpulseResponse(file, juce::dsp::Convolution::Stereo::yes,
                                     juce::dsp::Convolution::Trim::yes, 0,
                                     juce::dsp::Convolution::Normalise::yes);
    irALoaded = true;
    bypassed = false;
    return true;
  }

  bool loadIRB(const juce::File &file) {
    if (!file.existsAsFile())
      return false;
    convolutionB.loadImpulseResponse(file, juce::dsp::Convolution::Stereo::yes,
                                     juce::dsp::Convolution::Trim::yes, 0,
                                     juce::dsp::Convolution::Normalise::yes);
    irBLoaded = true;
    bypassed = false;
    return true;
  }

  void setMix(float m) { mix = juce::jlimit(0.0f, 1.0f, m); }

  bool isALoaded() const { return irALoaded; }
  bool isBLoaded() const { return irBLoaded; }

private:
  void processConvolution(juce::dsp::Convolution &conv,
                          juce::AudioBuffer<float> &buffer) {
    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> context(block);
    conv.process(context);
  }

  juce::dsp::Convolution convolutionA;
  juce::dsp::Convolution convolutionB;
  juce::AudioBuffer<float> tempBuffer;

  float mix = 0.5f; // 0 = A, 1 = B
  bool irALoaded = false;
  bool irBLoaded = false;
};
