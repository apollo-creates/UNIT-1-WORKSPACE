#pragma once

#include <functional>
#include <juce_audio_processors/juce_audio_processors.h>
#include <map>
#include <mutex>

class PluginHostManager : private juce::AudioProcessorListener,
                          private juce::Timer {
public:
  // Transport Emulation
  class DummyPlayHead : public juce::AudioPlayHead {
  public:
    DummyPlayHead() = default;
    juce::Optional<juce::AudioPlayHead::PositionInfo>
    getPosition() const override {
      juce::AudioPlayHead::PositionInfo info;
      info.setIsPlaying(true);
      info.setBpm(120.0);
      info.setTimeInSamples(0);
      info.setTimeInSeconds(0);
      return info;
    }
  };
  PluginHostManager();
  ~PluginHostManager() override;

  bool loadPlugin(const juce::String &path, double sampleRate, int blockSize);
  void unloadPlugin();

  juce::AudioPluginInstance *getActivePlugin() { return activePlugin.get(); }

  // Editor management
  void showEditor();
  void hideEditor();
  bool hasEditor() const;

  // Parameter handling
  int getNumParameters() const;
  juce::String getParameterName(int index) const;
  void setParameterValue(int index, float newValue);
  float getParameterValue(int index) const;

  // Learn callback
  std::function<void(int, float)> onParameterChanged;

private:
  // AudioProcessorListener methods
  void audioProcessorParameterChanged(juce::AudioProcessor *,
                                      int parameterIndex,
                                      float newValue) override;
  void audioProcessorChanged(juce::AudioProcessor *,
                             const ChangeDetails &) override {}

  // Timer method for headless pumping
  void timerCallback() override;

  std::unique_ptr<juce::AudioPluginInstance> activePlugin;
  juce::AudioPluginFormatManager formatManager;
  DummyPlayHead playHead;

  // MIDI Bridge storage: CC -> Value (0-1)
  std::map<int, float> pendingMidiCCs;
  std::mutex midiMutex;
  float currentCCValues[128] = {0.0f};

  struct EditorWindow : public juce::DocumentWindow {
    EditorWindow(juce::String name, juce::AudioProcessorEditor *editor)
        : DocumentWindow(name, juce::Colours::darkgrey,
                         DocumentWindow::allButtons) {
      setUsingNativeTitleBar(true);
      setContentOwned(editor, true);
      setResizable(editor->isResizable(), editor->isResizable());

      if (auto *const topLevel = juce::TopLevelWindow::getTopLevelWindow(0))
        centreAroundComponent(topLevel, getWidth(), getHeight());
      else
        centreWithSize(getWidth(), getHeight());

      setVisible(true);
    }

    void closeButtonPressed() override { setVisible(false); }
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EditorWindow)
  };

  std::unique_ptr<EditorWindow> editorWindow;
};
