#include "PluginHostManager.h"

PluginHostManager::PluginHostManager() { formatManager.addDefaultFormats(); }

PluginHostManager::~PluginHostManager() { unloadPlugin(); }

bool PluginHostManager::loadPlugin(const juce::String &path, double sampleRate,
                                   int blockSize) {
  unloadPlugin();

  juce::KnownPluginList pluginList;
  juce::File file(path);

  if (!file.exists())
    return false;

  juce::OwnedArray<juce::PluginDescription> descriptions;
  for (int i = 0; i < formatManager.getNumFormats(); ++i) {
    auto format = formatManager.getFormat(i);
    format->findAllTypesForFile(descriptions, path);
  }

  if (descriptions.size() == 0)
    return false;

  juce::String error;
  activePlugin = formatManager.createPluginInstance(
      *descriptions[0], sampleRate, blockSize, error);

  if (activePlugin != nullptr) {
    activePlugin->addListener(this);
    activePlugin->setPlayHead(&playHead);
    activePlugin->prepareToPlay(sampleRate, blockSize);
    startTimer(40); // 25Hz headless pumping
    return true;
  }

  return false;
}

void PluginHostManager::unloadPlugin() {
  stopTimer();
  hideEditor();

  if (activePlugin != nullptr) {
    activePlugin->removeListener(this);
    activePlugin->releaseResources();
    activePlugin = nullptr;
  }
}

void PluginHostManager::showEditor() {
  if (activePlugin != nullptr) {
    if (editorWindow != nullptr) {
      editorWindow->setVisible(true);
      editorWindow->toFront(true);
    } else if (activePlugin->hasEditor()) {
      auto *editor = activePlugin->createEditorIfNeeded();
      if (editor != nullptr) {
        editorWindow =
            std::make_unique<EditorWindow>(activePlugin->getName(), editor);
      }
    }
  }
}

void PluginHostManager::hideEditor() { editorWindow = nullptr; }

bool PluginHostManager::hasEditor() const {
  return activePlugin != nullptr && activePlugin->hasEditor();
}

int PluginHostManager::getNumParameters() const {
  if (activePlugin != nullptr)
    return activePlugin->getParameters().size();

  return 0;
}

juce::String PluginHostManager::getParameterName(int index) const {
  if (activePlugin != nullptr &&
      index < (int)activePlugin->getParameters().size()) {
    auto *param = activePlugin->getParameters()[index];
    return param->getName(128);
  }

  return {};
}

void PluginHostManager::setParameterValue(int index, float newValue) {
  if (activePlugin != nullptr &&
      index < (int)activePlugin->getParameters().size()) {
    auto *param = activePlugin->getParameters()[index];
    param->beginChangeGesture();
    param->setValueNotifyingHost(newValue);
    param->endChangeGesture();

    // MIDI Bridge: Update CC value (Map slot index + 1 to CC)
    {
      const std::lock_guard<std::mutex> lock(midiMutex);
      pendingMidiCCs[index + 1] = newValue; // CC 1, CC 2...
    }
  }
}

float PluginHostManager::getParameterValue(int index) const {
  if (activePlugin != nullptr &&
      index < (int)activePlugin->getParameters().size()) {
    auto *param = activePlugin->getParameters()[index];
    return param->getValue();
  }

  return 0.0f;
}

void PluginHostManager::audioProcessorParameterChanged(
    juce::AudioProcessor *processor, int parameterIndex, float newValue) {
  if (onParameterChanged)
    onParameterChanged(parameterIndex, newValue);
}

void PluginHostManager::timerCallback() {
  if (activePlugin != nullptr) {
    auto numChannels = juce::jmax(activePlugin->getTotalNumInputChannels(),
                                  activePlugin->getTotalNumOutputChannels());
    if (numChannels > 0) {
      juce::AudioBuffer<float> buffer(numChannels, 512);
      buffer.clear();
      juce::MidiBuffer midi;

      // Inject pending MIDI CCs
      {
        const std::lock_guard<std::mutex> lock(midiMutex);
        for (auto const &[cc, val] : pendingMidiCCs) {
          int midiVal = juce::jlimit(0, 127, (int)(val * 127.0f));
          midi.addEvent(juce::MidiMessage::controllerEvent(1, cc, midiVal), 0);
        }
        pendingMidiCCs.clear();
      }

      activePlugin->processBlock(buffer, midi);
    }
  }
}
