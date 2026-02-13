#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Utils/ModelLoader.h"

UNIT1ModellerAudioProcessor::UNIT1ModellerAudioProcessor()
    : AudioProcessor(
          BusesProperties()
              .withInput("Input", juce::AudioChannelSet::stereo(), true)
              .withOutput("Output", juce::AudioChannelSet::stereo(), true)) {}

UNIT1ModellerAudioProcessor::~UNIT1ModellerAudioProcessor() {}

void UNIT1ModellerAudioProcessor::prepareToPlay(double sampleRate,
                                                int samplesPerBlock) {
  pedal.prepare(sampleRate, samplesPerBlock);
  amp.prepare(sampleRate, samplesPerBlock);
  cab.prepare(sampleRate, samplesPerBlock);
}

void UNIT1ModellerAudioProcessor::releaseResources() {}

void UNIT1ModellerAudioProcessor::processBlock(
    juce::AudioBuffer<float> &buffer, juce::MidiBuffer & /*midiMessages*/) {
  juce::ScopedNoDenormals noDenormals;
  auto totalNumInputChannels = getTotalNumInputChannels();
  auto totalNumOutputChannels = getTotalNumOutputChannels();

  for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
    buffer.clear(i, 0, buffer.getNumSamples());

  pedal.process(buffer);
  amp.process(buffer);
  cab.process(buffer);
}

void UNIT1ModellerAudioProcessor::loadPedalModel(const juce::File &file) {
  ModelLoader::loadPedalWeights(file, pedal);
}

void UNIT1ModellerAudioProcessor::loadAmpModel(const juce::File &file) {
  ModelLoader::loadAmpWeights(file, amp);
}

void UNIT1ModellerAudioProcessor::loadCabinetIRA(const juce::File &file) {
  cab.loadIRA(file);
}

void UNIT1ModellerAudioProcessor::loadCabinetIRB(const juce::File &file) {
  cab.loadIRB(file);
}

juce::AudioProcessorEditor *UNIT1ModellerAudioProcessor::createEditor() {
  return new UNIT1ModellerAudioProcessorEditor(*this);
}

void UNIT1ModellerAudioProcessor::getStateInformation(
    juce::MemoryBlock & /*destData*/) {}

void UNIT1ModellerAudioProcessor::setStateInformation(const void * /*data*/,
                                                      int /*sizeInBytes*/) {}

juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
  return new UNIT1ModellerAudioProcessor();
}
