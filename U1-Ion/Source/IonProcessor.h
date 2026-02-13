#pragma once

#include "DSP/CabSimulator.h"
#include "DSP/HorizonDrive.h"
#include "DSP/PVVitriol.h"
#include <atomic>
#include <juce_audio_processors/juce_audio_processors.h>

class IonProcessor : public juce::AudioProcessor {
public:
  IonProcessor();
  ~IonProcessor() override;

  void prepareToPlay(double sampleRate, int samplesPerBlock) override;
  void releaseResources() override;
  void processBlock(juce::AudioBuffer<float> &, juce::MidiBuffer &) override;

  juce::AudioProcessorEditor *createEditor() override;
  bool hasEditor() const override { return true; }

  const juce::String getName() const override { return "U1 Ion"; }

  bool acceptsMidi() const override { return false; }
  bool producesMidi() const override { return false; }
  bool isMidiEffect() const override { return false; }
  double getTailLengthSeconds() const override { return 0.0; }

  int getNumPrograms() override { return 1; }
  int getCurrentProgram() override { return 0; }
  void setCurrentProgram(int) override {}
  const juce::String getProgramName(int) override { return {}; }
  void changeProgramName(int, const juce::String &) override {}

  void getStateInformation(juce::MemoryBlock &) override;
  void setStateInformation(const void *, int) override;

  // DSP Controls
  void setPowerOn(bool on) { powerOn = on; }
  bool isPowerOn() const { return powerOn; }

  void setInputLevel(float level) { inputGain = level; }
  void setOutputLevel(float level) { outputGain = level; }

  float getInputMeterLevel() const { return inputMeter.load(); }
  float getOutputMeterLevel() const { return outputMeter.load(); }

  void loadPedalModel(const juce::File &file);
  void loadAmpModel(const juce::File &file);
  void loadCabinetIRA(const juce::File &file);
  void loadCabinetIRB(const juce::File &file);

  HorizonDrive &getPedal() { return pedal; }
  PVVitriolAmp &getAmp() { return amp; }
  CabSimulator &getCab() { return cab; }

private:
  HorizonDrive pedal;
  PVVitriolAmp amp;
  CabSimulator cab;

  bool powerOn = true;
  float inputGain = 1.0f;  // Unity by default
  float outputGain = 1.0f; // Unity by default

  std::atomic<float> inputMeter{0.0f}, outputMeter{0.0f};

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IonProcessor)
};
