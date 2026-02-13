#include "IonProcessor.h"
#include "BinaryData.h"
#include "IonEditor.h"
#include "Utils/ModelLoader.h"

IonProcessor::IonProcessor()
    : AudioProcessor(
          BusesProperties()
              .withInput("Input", juce::AudioChannelSet::stereo(), true)
              .withOutput("Output", juce::AudioChannelSet::stereo(), true)) {

  // Load Default Embedded Models
  ModelLoader::loadPedalFromBinary(
      BinaryData::v003_01_Horizon_Pedal_Epochs1000_Chunk60s_Loss0p006719_json,
      BinaryData::
          v003_01_Horizon_Pedal_Epochs1000_Chunk60s_Loss0p006719_jsonSize,
      pedal);
  ModelLoader::loadAmpFromBinary(
      BinaryData::v005_02_Invective_Amp_Epochs300_Chunk2s_Loss0p002201_json,
      BinaryData::v005_02_Invective_Amp_Epochs300_Chunk2s_Loss0p002201_jsonSize,
      amp);
}

IonProcessor::~IonProcessor() {}

void IonProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
  pedal.prepare(sampleRate, samplesPerBlock);
  amp.prepare(sampleRate, samplesPerBlock);
  cab.prepare(sampleRate, samplesPerBlock);
}

void IonProcessor::releaseResources() {}

void IonProcessor::processBlock(juce::AudioBuffer<float> &buffer,
                                juce::MidiBuffer &) {
  juce::ScopedNoDenormals noDenormals;

  if (!powerOn) {
    buffer.clear();
    inputMeter.store(0.0f);
    outputMeter.store(0.0f);
    return;
  }

  // 1. Input Gain (Initial Unity)
  buffer.applyGain(inputGain);

  // Input Meter (Simple peak for now, Logarithmic mapping)
  float inPeak = buffer.getMagnitude(0, buffer.getNumSamples());
  float inLevel = inPeak > 0.001f ? (0.2f * std::log10(inPeak) + 1.0f) : 0.0f;
  inputMeter.store(juce::jlimit(0.0f, 1.0f, inLevel));

  // 2. Processing Chain
  pedal.process(buffer);
  amp.process(buffer);
  cab.process(buffer);

  // 3. Output Gain (Initial Unity)
  buffer.applyGain(outputGain);

  // Output Meter
  float outPeak = buffer.getMagnitude(0, buffer.getNumSamples());
  float outLevel =
      outPeak > 0.001f ? (0.2f * std::log10(outPeak) + 1.0f) : 0.0f;
  outputMeter.store(juce::jlimit(0.0f, 1.0f, outLevel));
}

void IonProcessor::loadPedalModel(const juce::File &file) {
  ModelLoader::loadPedalWeights(file, pedal);
}
void IonProcessor::loadAmpModel(const juce::File &file) {
  ModelLoader::loadAmpWeights(file, amp);
}
void IonProcessor::loadCabinetIRA(const juce::File &file) { cab.loadIRA(file); }
void IonProcessor::loadCabinetIRB(const juce::File &file) { cab.loadIRB(file); }

juce::AudioProcessorEditor *IonProcessor::createEditor() {
  return new IonEditor(*this);
}

void IonProcessor::getStateInformation(juce::MemoryBlock &) {}
void IonProcessor::setStateInformation(const void *, int) {}

juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter() {
  return new IonProcessor();
}
