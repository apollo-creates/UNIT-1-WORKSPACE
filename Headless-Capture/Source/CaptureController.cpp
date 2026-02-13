#include "CaptureController.h"
#include <juce_audio_formats/juce_audio_formats.h>
#include <thread>

CaptureController::CaptureController() { formatManager.registerBasicFormats(); }

CaptureController::~CaptureController() { stopCapture(); }

void CaptureController::startCapture(
    juce::AudioPluginInstance *plugin, const juce::File &inputFile,
    const juce::File &outputDir, double chunkTimeSeconds,
    const juce::Array<MappedParameter> &mappedParams, bool isAmp,
    const juce::Array<CapturePass> &batchPasses) {
  if (isCapturing)
    return;

  isCapturing = true;
  shouldStop = false;

  // 0.0 means Full Render
  int samplesPerChunk = (chunkTimeSeconds <= 0.0)
                            ? 0
                            : static_cast<int>(chunkTimeSeconds * sampleRate);

  std::thread([this, plugin, inputFile, outputDir, samplesPerChunk,
               mappedParams, isAmp, batchPasses] {
    performRenderingLoop(plugin, inputFile, outputDir, samplesPerChunk,
                         mappedParams, isAmp, batchPasses);
  }).detach();
}

void CaptureController::stopCapture() { shouldStop = true; }

void CaptureController::performRenderingLoop(
    juce::AudioPluginInstance *plugin, const juce::File &inputFile,
    const juce::File &outputDir, int samplesPerChunk,
    const juce::Array<MappedParameter> &mappedParams, bool isAmp,
    const juce::Array<CapturePass> &batchPasses) {
  std::unique_ptr<juce::AudioFormatReader> reader(
      formatManager.createReaderFor(inputFile));

  if (reader == nullptr) {
    isCapturing = false;
    return;
  }

  // If samplesPerChunk is 0, we render the FULL file
  int actualSamplesPerChunk = (samplesPerChunk == 0)
                                  ? static_cast<int>(reader->lengthInSamples)
                                  : samplesPerChunk;

  outputDir.createDirectory();

  juce::DynamicObject::Ptr manifest = new juce::DynamicObject();
  manifest->setProperty("type", isAmp ? "AMP" : "PEDAL");
  manifest->setProperty("sample_rate", sampleRate);

  juce::Array<juce::var> chunks;

  bool isBatchMode = batchPasses.size() > 0;
  int totalSteps =
      isBatchMode ? batchPasses.size() : ((mappedParams.size() > 0) ? 10 : 1);

  int numChannels =
      juce::jmax(2, juce::jmax(plugin->getTotalNumInputChannels(),
                               plugin->getTotalNumOutputChannels()));

  juce::AudioBuffer<float> inBuffer(numChannels, actualSamplesPerChunk);
  juce::AudioBuffer<float> outBuffer(numChannels, actualSamplesPerChunk);
  inBuffer.clear();
  outBuffer.clear();

  for (int i = 0; i < totalSteps; ++i) {
    if (shouldStop)
      break;

    float progress = (float)i / (float)totalSteps;
    juce::String status = isBatchMode ? "Batch Pass " + juce::String(i + 1)
                                      : "Capturing Step " + juce::String(i + 1);
    if (OnProgressUpdate)
      OnProgressUpdate(progress,
                       status + (samplesPerChunk == 0 ? " (Full)" : ""));

    // 1. Reset ALL mapped parameters to default (0.5) first
    {
      auto &params = plugin->getParameters();
      for (auto &mp : mappedParams) {
        if (mp.paramIndex < (int)params.size()) {
          auto *p = params[mp.paramIndex];
          p->beginChangeGesture();
          p->setValueNotifyingHost(0.5f);
          p->endChangeGesture();
        }
      }
    }

    // 2. Set Plugin Parameters for this specific pass
    if (isBatchMode) {
      const auto &pass = batchPasses[i];
      auto &params = plugin->getParameters();
      for (auto &mp : mappedParams) {
        if (pass.settings.contains(mp.name)) {
          float val = (float)pass.settings.getWithDefault(mp.name, 0.5f);
          if (mp.paramIndex < (int)params.size()) {
            auto *p = params[mp.paramIndex];
            p->beginChangeGesture();
            p->setValueNotifyingHost(val);
            p->endChangeGesture();
          }
        }
      }
    } else {
      for (auto &mp : mappedParams) {
        float normalizedT = (float)i / (float)(totalSteps - 1);
        if (mp.isStepped) {
          normalizedT =
              std::round(normalizedT * (totalSteps - 1)) / (totalSteps - 1);
        }
        float actualVal = mp.minVal + (normalizedT * (mp.maxVal - mp.minVal));
        auto &params = plugin->getParameters();
        if (mp.paramIndex < (int)params.size()) {
          auto *p = params[mp.paramIndex];
          p->beginChangeGesture();
          p->setValueNotifyingHost(actualVal);
          p->endChangeGesture();
        }
      }
    }

    // 2. Read Input Chunk
    int startSample = 0;
    if (samplesPerChunk > 0)
      startSample = (i * samplesPerChunk) %
                    (int)(reader->lengthInSamples - samplesPerChunk);

    inBuffer.clear();
    reader->read(&inBuffer, 0, actualSamplesPerChunk, startSample, true, true);

    // 3. Process in fixed blocks (Safety first for plugins!)
    const int maxBlockSize = 512;
    plugin->prepareToPlay(sampleRate, maxBlockSize);

    juce::AudioBuffer<float> blockBuffer(numChannels, maxBlockSize);
    int samplesProcessed = 0;
    while (samplesProcessed < actualSamplesPerChunk) {
      if (shouldStop)
        break;

      int samplesToProcess =
          std::min(maxBlockSize, actualSamplesPerChunk - samplesProcessed);

      blockBuffer.clear();
      for (int ch = 0; ch < numChannels; ++ch) {
        if (ch < inBuffer.getNumChannels())
          blockBuffer.copyFrom(ch, 0, inBuffer, ch, samplesProcessed,
                               samplesToProcess);
      }

      juce::MidiBuffer midi;
      plugin->processBlock(blockBuffer, midi);

      for (int ch = 0; ch < numChannels; ++ch) {
        if (ch < outBuffer.getNumChannels())
          outBuffer.copyFrom(ch, samplesProcessed, blockBuffer, ch, 0,
                             samplesToProcess);
      }

      samplesProcessed += samplesToProcess;
    }

    // 4. Save
    juce::String fileName = isBatchMode ? batchPasses[i].filename
                                        : "capture_" + juce::String(i) + ".wav";
    juce::File targetDir =
        isBatchMode ? outputDir.getChildFile(batchPasses[i].targetDir)
                    : outputDir.getChildFile("targets");
    targetDir.createDirectory();
    juce::File outFile = targetDir.getChildFile(fileName);

    juce::WavAudioFormat wavFormat;
    std::unique_ptr<juce::AudioFormatWriter> writer(wavFormat.createWriterFor(
        new juce::FileOutputStream(outFile), sampleRate,
        static_cast<int>(reader->numChannels), 16, {}, 0));
    if (writer != nullptr) {
      writer->writeFromAudioSampleBuffer(outBuffer, 0, actualSamplesPerChunk);
      writer = nullptr;
    }

    // 5. Manifest
    juce::DynamicObject::Ptr entry = new juce::DynamicObject();
    entry->setProperty("file", fileName);
    juce::DynamicObject::Ptr paramsObj = new juce::DynamicObject();
    for (auto &mp : mappedParams) {
      auto &params = plugin->getParameters();
      if (mp.paramIndex < (int)params.size())
        paramsObj->setProperty(mp.name, params[mp.paramIndex]->getValue());
    }
    entry->setProperty("params", paramsObj.get());
    chunks.add(entry.get());
  }

  manifest->setProperty("chunks", chunks);
  juce::File manifestFile = outputDir.getChildFile("manifest.json");
  manifestFile.replaceWithText(juce::JSON::toString(manifest.get()));

  isCapturing = false;
  if (OnProgressUpdate)
    OnProgressUpdate(1.0f, "Capture Complete");
}
