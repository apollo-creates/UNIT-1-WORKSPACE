#pragma once

#include "CaptureController.h"
#include "PluginHostManager.h"
#include <juce_gui_extra/juce_gui_extra.h>

class MainComponent : public juce::Component, public juce::Timer {
public:
  MainComponent();
  ~MainComponent() override;

  void paint(juce::Graphics &) override;
  void resized() override;

private:
  void handleLoadPlugin();
  void handleStartCapture();
  void proceedWithCapture(const juce::File &waveFile,
                          const juce::File &outputDir);
  void handleShowGUI();
  void handleImportTrainerConfig();
  void updateParameterList();
  void mapSelectedToSlot(int slotIndex);
  void updateSlotCount();
  void handleDryRun();
  void timerCallback() override;

  struct ParameterListModel : public juce::ListBoxModel {
    ParameterListModel(PluginHostManager &hm) : hostManager(hm) {}
    ~ParameterListModel() override = default;
    int getNumRows() override;
    void paintListBoxItem(int rowNumber, juce::Graphics &g, int width,
                          int height, bool rowIsSelected) override;
    PluginHostManager &hostManager;
  };

  struct MappingSlot : public juce::Component {
    MappingSlot(int id);
    ~MappingSlot() override = default;
    void paint(juce::Graphics &g) override;
    void resized() override;

    int slotId;
    int mappedParamIndex = -1;
    juce::String mappedParamName = "Unmapped";
    juce::TextEditor nameEditor;
    juce::TextButton mapButton{"Map"};
    juce::ToggleButton learnButton{"Learn"};
    juce::Slider testSlider{juce::Slider::LinearHorizontal,
                            juce::Slider::NoTextBox};
    juce::Label testLabel{{}, "Test:"};
    juce::TextEditor minEdit, maxEdit;
    juce::Label minLabel{{}, "Min:"}, maxLabel{{}, "Max:"};
    juce::ComboBox typeBox;

    std::function<void(int)> onMapRequest;
    std::function<void(int, bool)> onLearnToggled;
    std::function<void(int, float)> onSliderMoved;
  };

  juce::Label titleLabel;
  juce::TextButton loadPluginButton{"Load VST3/AU Plugin"};
  juce::TextButton showGUIButton{"Show Plugin GUI"};
  juce::TextButton importTrainerButton{"Import Trainer Config"};
  juce::TextButton startCaptureButton{"Start Offline Capture"};
  juce::TextButton dryRunButton{"Dry Run Batch"};

  juce::ComboBox timeSelection;
  juce::Label timeLabel{{}, "Chunk Time:"};
  juce::ComboBox slotCountSelection;
  juce::Label slotCountLabel{{}, "Num Knobs:"};

  juce::ProgressBar progressBar;
  double captureProgress = 0.0;
  juce::String statusText = "Ready";

  juce::ListBox parameterList;
  std::unique_ptr<ParameterListModel> listModel;
  juce::OwnedArray<MappingSlot> mappingSlots;
  juce::Viewport slotViewport;
  juce::Component slotContainer;

  PluginHostManager hostManager;
  CaptureController captureController;
  std::unique_ptr<juce::FileChooser> chooser;

  juce::File referenceFile; // New: stores currently selected/default WAV
  juce::File presetOutputDir;
  juce::String presetProjectName;
  juce::Array<CaptureController::CapturePass> batchPasses;
  int dryRunPassIndex = 0;
  bool isDryRunActive = false;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
