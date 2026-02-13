#pragma once

#include <juce_audio_formats/juce_audio_formats.h>
#include <juce_audio_processors/juce_audio_processors.h>

class CaptureController {
public:
  struct MappedParameter {
    int paramIndex;
    juce::String name;
    float minVal = 0.0f;
    float maxVal = 1.0f;
    bool isStepped = false;
  };

  struct CapturePass {
    juce::String filename;
    juce::String targetDir;
    juce::NamedValueSet settings;
  };

  CaptureController();
  ~CaptureController();

  // Callback to update progress in UI
  std::function<void(float, juce::String)> OnProgressUpdate;

  void startCapture(juce::AudioPluginInstance *plugin,
                    const juce::File &inputFile, const juce::File &outputDir,
                    double chunkTimeSeconds,
                    const juce::Array<MappedParameter> &mappedParams,
                    bool isAmp,
                    const juce::Array<CapturePass> &batchPasses = {});

  void stopCapture();
  bool isActive() const { return isCapturing; }

private:
  void performRenderingLoop(juce::AudioPluginInstance *plugin,
                            const juce::File &inputFile,
                            const juce::File &outputDir, int samplesPerChunk,
                            const juce::Array<MappedParameter> &mappedParams,
                            bool isAmp,
                            const juce::Array<CapturePass> &batchPasses);

  std::atomic<bool> isCapturing{false};
  std::atomic<bool> shouldStop{false};
  double sampleRate = 48000.0;

  juce::AudioFormatManager formatManager;
};
