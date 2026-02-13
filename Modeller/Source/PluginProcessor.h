#pragma once

#include "DSP/CabSimulator.h"
#include "DSP/HorizonDrive.h"
#include "DSP/PVVitriol.h"
#include <juce_audio_processors/juce_audio_processors.h>

class UNIT1ModellerAudioProcessor : public juce::AudioProcessor {
public:
  UNIT1ModellerAudioProcessor();
  ~UNIT1ModellerAudioProcessor() override;

  void prepareToPlay(double sampleRate, int samplesPerBlock) override;
  void releaseResources() override;
  void processBlock(juce::AudioBuffer<float> &, juce::MidiBuffer &) override;

  juce::AudioProcessorEditor *createEditor() override;
  bool hasEditor() const override { return true; }

  const juce::String getName() const override { return "UNIT 1 Modeller"; }

  bool acceptsMidi() const override { return false; }
  bool producesMidi() const override { return false; }
  bool isMidiEffect() const override { return false; }
  double getTailLengthSeconds() const override { return 0.0; }

  int getNumPrograms() override { return 1; }
  int getCurrentProgram() override { return 0; }
  void setCurrentProgram(int /*index*/) override {}
  const juce::String getProgramName(int /*index*/) override { return {}; }
  void changeProgramName(int /*index*/,
                         const juce::String & /*newName*/) override {}

  void getStateInformation(juce::MemoryBlock &destData) override;
  void setStateInformation(const void *data, int sizeInBytes) override;

  // DSP Interface
  void loadPedalModel(const juce::File &file);
  void loadAmpModel(const juce::File &file);
  void loadCabinetIRA(const juce::File &file);
  void loadCabinetIRB(const juce::File &file);

  // Direct Access for sliders (Simple Implementation)
  HorizonDrive &getPedal() { return pedal; }
  PVVitriolAmp &getAmp() { return amp; }
  CabSimulator &getCab() { return cab; }

private:
  // DSP Blocks
  HorizonDrive pedal;
  PVVitriolAmp amp;
  CabSimulator cab;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(UNIT1ModellerAudioProcessor)
};
