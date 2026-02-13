#include "MainComponent.h"

// --- MappingSlot Implementation ---
MainComponent::MappingSlot::MappingSlot(int id) : slotId(id) {
  addAndMakeVisible(nameEditor);
  nameEditor.setText("Slot " + juce::String(id + 1));
  nameEditor.setColour(juce::TextEditor::backgroundColourId,
                       juce::Colours::transparentBlack);
  nameEditor.setColour(juce::TextEditor::outlineColourId,
                       juce::Colours::transparentBlack);

  addAndMakeVisible(mapButton);
  mapButton.onClick = [this] {
    if (onMapRequest)
      onMapRequest(slotId);
  };

  addAndMakeVisible(learnButton);
  learnButton.onStateChange = [this] {
    if (onLearnToggled)
      onLearnToggled(slotId, learnButton.getToggleState());
  };

  addAndMakeVisible(testSlider);
  testSlider.setRange(0.0, 1.0);
  testSlider.onValueChange = [this] {
    if (onSliderMoved)
      onSliderMoved(slotId, (float)testSlider.getValue());
  };

  addAndMakeVisible(minEdit);
  minEdit.setText("0.0");
  addAndMakeVisible(maxEdit);
  maxEdit.setText("1.0");

  addAndMakeVisible(typeBox);
  typeBox.addItem("Linear", 1);
  typeBox.addItem("Stepped", 2);
  typeBox.setSelectedId(1);

  addAndMakeVisible(testLabel);
  addAndMakeVisible(minLabel);
  addAndMakeVisible(maxLabel);
}

void MainComponent::MappingSlot::paint(juce::Graphics &g) {
  g.setColour(juce::Colours::black.withAlpha(0.4f));
  g.fillRoundedRectangle(getLocalBounds().toFloat(), 5.0f);

  g.setColour(juce::Colours::white);
  g.setFont(12.0f);
  g.drawText("#" + juce::String(slotId + 1), 5, 5, 30, 20,
             juce::Justification::centredLeft);

  if (mappedParamIndex >= 0) {
    g.setColour(juce::Colours::gold);
    g.fillEllipse(40, 11, 8, 8);

    g.setColour(juce::Colours::white.withAlpha(0.7f));
    g.setFont(10.0f);
    g.drawText(mappedParamName + " (MIDI CC " +
                   juce::String(mappedParamIndex + 1) + ")",
               210, 5, 200, 20, juce::Justification::centredLeft);
  }

  if (learnButton.getToggleState()) {
    g.setColour(juce::Colours::red.withAlpha(0.3f));
    g.drawRoundedRectangle(getLocalBounds().toFloat(), 5.0f, 2.0f);
  }
}

void MainComponent::MappingSlot::resized() {
  auto area = getLocalBounds().reduced(5);
  auto topArea = area.removeFromTop(25);
  topArea.removeFromLeft(50);
  nameEditor.setBounds(topArea.removeFromLeft(150));

  auto leftArea = area.removeFromLeft(180);
  auto rangeRow = leftArea.removeFromTop(25);
  minLabel.setBounds(rangeRow.removeFromLeft(40));
  minEdit.setBounds(rangeRow.removeFromLeft(45));
  maxLabel.setBounds(rangeRow.removeFromLeft(40));
  maxEdit.setBounds(rangeRow.removeFromLeft(45));

  auto midArea = area.removeFromLeft(200);
  testLabel.setBounds(midArea.removeFromTop(15));
  testSlider.setBounds(midArea.removeFromTop(25));

  auto rightArea = area.removeFromRight(110);
  typeBox.setBounds(rightArea.removeFromTop(25).reduced(0, 2));
  mapButton.setBounds(rightArea.removeFromLeft(50));
  learnButton.setBounds(rightArea.removeFromLeft(50));
}

// --- ListBoxModel Implementation ---
int MainComponent::ParameterListModel::getNumRows() {
  return hostManager.getNumParameters();
}
void MainComponent::ParameterListModel::paintListBoxItem(int rowNumber,
                                                         juce::Graphics &g,
                                                         int width, int height,
                                                         bool rowIsSelected) {
  if (rowIsSelected)
    g.fillAll(juce::Colours::lightblue.withAlpha(0.3f));
  g.setColour(juce::Colours::white.withAlpha(0.9f));
  g.setFont(12.0f);
  g.drawText(hostManager.getParameterName(rowNumber), 10, 0, width - 10, height,
             juce::Justification::centredLeft, true);
}

// --- MainComponent Implementation ---
MainComponent::MainComponent() : progressBar(captureProgress) {
  addAndMakeVisible(titleLabel);
  titleLabel.setText("UNIT 1 - HEADLESS CAPTURE", juce::dontSendNotification);
  titleLabel.setFont(juce::FontOptions(24.0f).withStyle("Bold"));

  addAndMakeVisible(loadPluginButton);
  loadPluginButton.onClick = [this] { handleLoadPlugin(); };

  addAndMakeVisible(showGUIButton);
  showGUIButton.onClick = [this] { handleShowGUI(); };
  showGUIButton.setEnabled(false);

  addAndMakeVisible(importTrainerButton);
  importTrainerButton.onClick = [this] { handleImportTrainerConfig(); };
  importTrainerButton.setColour(juce::TextButton::buttonColourId,
                                juce::Colours::darkgrey);

  addAndMakeVisible(startCaptureButton);
  startCaptureButton.onClick = [this] { handleStartCapture(); };
  startCaptureButton.setColour(juce::TextButton::buttonColourId,
                               juce::Colours::darkred);

  addAndMakeVisible(dryRunButton);
  dryRunButton.onClick = [this] { handleDryRun(); };
  dryRunButton.setColour(juce::TextButton::buttonColourId,
                         juce::Colours::darkblue);

  addAndMakeVisible(parameterList);
  listModel = std::make_unique<ParameterListModel>(hostManager);
  parameterList.setModel(listModel.get());

  addAndMakeVisible(timeSelection);
  timeSelection.addItem("2 Seconds", 1);
  timeSelection.addItem("5 Seconds", 2);
  timeSelection.addItem("10 Seconds", 3);
  timeSelection.addItem("30 Seconds", 4);
  timeSelection.addItem("60 Seconds", 5);
  timeSelection.addItem("120 Seconds", 6);
  timeSelection.addItem("Full Render", 7);
  timeSelection.setSelectedId(2);

  addAndMakeVisible(slotCountSelection);
  for (int i = 1; i <= 16; ++i)
    slotCountSelection.addItem(juce::String(i), i);
  slotCountSelection.setSelectedId(4);
  slotCountSelection.onChange = [this] { updateSlotCount(); };

  addAndMakeVisible(timeLabel);
  addAndMakeVisible(slotCountLabel);
  addAndMakeVisible(progressBar);

  addAndMakeVisible(slotViewport);
  slotViewport.setScrollBarsShown(true, false);
  slotViewport.setViewedComponent(&slotContainer);

  hostManager.onParameterChanged = [this](int index, float val) {
    juce::MessageManager::callAsync([this, index, val] {
      for (auto *slot : mappingSlots)
        if (slot->learnButton.getToggleState()) {
          slot->mappedParamIndex = index;
          slot->mappedParamName = hostManager.getParameterName(index);
          slot->testSlider.setValue(val, juce::dontSendNotification);
          slot->learnButton.setToggleState(false, juce::sendNotification);
          slot->repaint();
          break;
        }
    });
  };

  captureController.OnProgressUpdate = [this](float progress,
                                              juce::String text) {
    juce::MessageManager::callAsync([this, progress, text] {
      captureProgress = progress;
      statusText = text;
      repaint();
    });
  };

  // Auto-detect input.wav
  juce::File exeDir =
      juce::File::getSpecialLocation(juce::File::currentExecutableFile)
          .getParentDirectory();
  juce::File localWav =
      juce::File::getCurrentWorkingDirectory().getChildFile("input.wav");
  if (localWav.existsAsFile()) {
    referenceFile = localWav;
    statusText = "Default Reference: input.wav";
  }

  updateSlotCount();
  setSize(1200, 850);
}

MainComponent::~MainComponent() { stopTimer(); }

void MainComponent::updateSlotCount() {
  int count = slotCountSelection.getSelectedId();
  mappingSlots.clear();
  slotContainer.removeAllChildren();

  for (int i = 0; i < count; ++i) {
    auto *slot = new MappingSlot(i);
    slot->onMapRequest = [this](int id) { mapSelectedToSlot(id); };
    slot->onLearnToggled = [this](int id, bool active) {
      if (active)
        for (auto *s : mappingSlots)
          if (s->slotId != id)
            s->learnButton.setToggleState(false, juce::dontSendNotification);
    };
    slot->onSliderMoved = [this](int id, float val) {
      auto *s = mappingSlots[id];
      if (s->mappedParamIndex >= 0)
        hostManager.setParameterValue(s->mappedParamIndex, val);
    };
    mappingSlots.add(slot);
    slotContainer.addAndMakeVisible(slot);
  }
  resized();
}

void MainComponent::handleImportTrainerConfig() {
  chooser = std::make_unique<juce::FileChooser>("Select Trainer Config JSON",
                                                juce::File{}, "*.json");
  chooser->launchAsync(
      juce::FileBrowserComponent::openMode |
          juce::FileBrowserComponent::canSelectFiles,
      [this](const juce::FileChooser &fc) {
        auto file = fc.getResult();
        if (!file.existsAsFile())
          return;

        juce::var config = juce::JSON::parse(file);
        if (config.isVoid())
          return;

        presetProjectName = config.getProperty("project_name", "Sync_Project");
        juce::String rootPath = config.getProperty("project_root", "");

        if (!rootPath.isEmpty()) {
          presetOutputDir = juce::File(rootPath);
          statusText = "Sync Active: " + presetProjectName;
        }

        auto *knobs = config.getProperty("knobs", juce::var()).getArray();
        if (knobs != nullptr) {
          slotCountSelection.setSelectedId(
              juce::jlimit(1, 16, (int)knobs->size()), juce::sendNotification);

          juce::MessageManager::callAsync([this, knobs_copy = *knobs] {
            for (int i = 0;
                 i < (int)knobs_copy.size() && i < mappingSlots.size(); ++i) {
              const juce::var k = knobs_copy[i];
              auto *slot = mappingSlots[i];
              slot->nameEditor.setText(k.getProperty("name", "Knob"));
              slot->minEdit.setText(
                  juce::String((float)k.getProperty("min", 0.0f), 2));
              slot->maxEdit.setText(
                  juce::String((float)k.getProperty("max", 1.0f), 2));
              slot->typeBox.setSelectedId(
                  k.getProperty("type", "") == "Stepped" ? 2 : 1);
            }
            repaint();
          });
        }

        // Parse Batch Passes
        batchPasses.clear();
        auto *passes = config.getProperty("passes", juce::var()).getArray();
        if (passes != nullptr) {
          for (auto &p : *passes) {
            CaptureController::CapturePass cp;
            cp.filename = p.getProperty("filename", "");
            cp.targetDir = p.getProperty("target_dir", "");

            auto *settings =
                p.getProperty("settings", juce::var()).getDynamicObject();
            if (settings != nullptr) {
              for (auto &s : settings->getProperties())
                cp.settings.set(s.name.toString(), (float)s.value);
            }
            batchPasses.add(cp);
          }
        }

        juce::AlertWindow::showMessageBoxAsync(
            juce::AlertWindow::InfoIcon, "Config Imported",
            "Roadmap loaded: " + presetProjectName +
                "\nTotal Passes: " + juce::String(batchPasses.size()));
        repaint();
      });
}

void MainComponent::handleLoadPlugin() {
  chooser = std::make_unique<juce::FileChooser>("Select Plugin", juce::File{},
                                                "*.vst3;*.component");
  chooser->launchAsync(
      juce::FileBrowserComponent::openMode |
          juce::FileBrowserComponent::canSelectFiles,
      [this](const juce::FileChooser &fc) {
        auto file = fc.getResult();
        if (file.exists() || file.isDirectory())
          if (hostManager.loadPlugin(file.getFullPathName(), 48000.0, 512)) {
            updateParameterList();
            showGUIButton.setEnabled(hostManager.hasEditor());

            // Auto-mapping logic
            auto fuzzyMatch = [](juce::String s1, juce::String s2) {
              auto clean = [](juce::String s) {
                return s.toLowerCase().replace("_", "").replace(" ", "");
              };
              return clean(s1) == clean(s2);
            };

            for (auto *slot : mappingSlots) {
              juce::String slotName = slot->nameEditor.getText();
              for (int i = 0; i < hostManager.getNumParameters(); ++i) {
                juce::String paramName = hostManager.getParameterName(i);
                if (fuzzyMatch(slotName, paramName)) {
                  slot->mappedParamIndex = i;
                  slot->mappedParamName = paramName;
                  slot->testSlider.setValue(hostManager.getParameterValue(i),
                                            juce::dontSendNotification);
                  slot->repaint();
                  break;
                }
              }
            }
          }
      });
}

void MainComponent::handleShowGUI() { hostManager.showEditor(); }

void MainComponent::handleStartCapture() {
  if (hostManager.getActivePlugin() == nullptr)
    return;

  if (referenceFile.existsAsFile()) {
    if (!presetOutputDir.getFullPathName().isEmpty()) {
      presetOutputDir.createDirectory();
      proceedWithCapture(referenceFile, presetOutputDir);
    } else {
      chooser = std::make_unique<juce::FileChooser>("Select Output Folder",
                                                    juce::File{}, "*");
      chooser->launchAsync(juce::FileBrowserComponent::openMode |
                               juce::FileBrowserComponent::canSelectDirectories,
                           [this](const juce::FileChooser &dirChooser) {
                             auto outputDir = dirChooser.getResult();
                             if (!outputDir.getFullPathName().isEmpty())
                               proceedWithCapture(referenceFile, outputDir);
                           });
    }
  } else {
    chooser = std::make_unique<juce::FileChooser>("Select Reference (.wav)",
                                                  juce::File{}, "*.wav");
    chooser->launchAsync(
        juce::FileBrowserComponent::openMode |
            juce::FileBrowserComponent::canSelectFiles,
        [this](const juce::FileChooser &waveChooser) {
          auto waveFile = waveChooser.getResult();
          if (!waveFile.existsAsFile())
            return;
          referenceFile = waveFile;

          if (!presetOutputDir.getFullPathName().isEmpty()) {
            presetOutputDir.createDirectory();
            proceedWithCapture(referenceFile, presetOutputDir);
          } else {
            chooser = std::make_unique<juce::FileChooser>(
                "Select Output Folder", juce::File{}, "*");
            chooser->launchAsync(
                juce::FileBrowserComponent::openMode |
                    juce::FileBrowserComponent::canSelectDirectories,
                [this](const juce::FileChooser &dirChooser) {
                  auto outputDir = dirChooser.getResult();
                  if (!outputDir.getFullPathName().isEmpty())
                    proceedWithCapture(referenceFile, outputDir);
                });
          }
        });
  }
}

void MainComponent::proceedWithCapture(const juce::File &waveFile,
                                       const juce::File &outputDir) {
  juce::Array<CaptureController::MappedParameter> mappings;
  for (auto *slot : mappingSlots) {
    if (slot->mappedParamIndex >= 0) {
      CaptureController::MappedParameter mp;
      mp.paramIndex = slot->mappedParamIndex;
      mp.name = slot->nameEditor.getText();
      mp.minVal = slot->minEdit.getText().getFloatValue();
      mp.maxVal = slot->maxEdit.getText().getFloatValue();
      mp.isStepped = (slot->typeBox.getSelectedId() == 2);
      mappings.add(mp);
    }
  }

  double time = 5.0;
  int mode = timeSelection.getSelectedId();
  if (mode == 1)
    time = 2.0;
  else if (mode == 2)
    time = 5.0;
  else if (mode == 3)
    time = 10.0;
  else if (mode == 4)
    time = 30.0;
  else if (mode == 5)
    time = 60.0;
  else if (mode == 6)
    time = 120.0;
  else if (mode == 7)
    time = 0.0;

  captureController.startCapture(hostManager.getActivePlugin(), waveFile,
                                 outputDir, time, mappings, true, batchPasses);
}

void MainComponent::handleDryRun() {
  if (isDryRunActive) {
    stopTimer();
    isDryRunActive = false;
    statusText = "Dry Run Cancelled";
    dryRunButton.setButtonText("Dry Run Batch");
    startCaptureButton.setEnabled(true);
    repaint();
    return;
  }

  if (batchPasses.isEmpty()) {
    juce::AlertWindow::showMessageBoxAsync(
        juce::AlertWindow::WarningIcon, "No Passes", "Import a config first!");
    return;
  }

  if (hostManager.getActivePlugin() == nullptr) {
    juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon,
                                           "No Plugin", "Load a plugin first!");
    return;
  }

  isDryRunActive = true;
  dryRunPassIndex = 0;
  dryRunButton.setButtonText("Stop Dry Run");
  startCaptureButton.setEnabled(false);
  startTimer(2000); // 2 second interval
  timerCallback();  // Start first pass immediately
}

void MainComponent::timerCallback() {
  if (dryRunPassIndex >= batchPasses.size()) {
    stopTimer();
    isDryRunActive = false;
    statusText = "Dry Run Finished";
    dryRunButton.setButtonText("Dry Run Batch");
    startCaptureButton.setEnabled(true);
    repaint();
    return;
  }

  const auto &pass = batchPasses[dryRunPassIndex];

  // 1. Reset all mapped parameters to 0.5f first
  for (auto *slot : mappingSlots) {
    if (slot->mappedParamIndex >= 0) {
      hostManager.setParameterValue(slot->mappedParamIndex, 0.5f);
      slot->testSlider.setValue(0.5f, juce::dontSendNotification);
    }
  }

  // 2. Apply settings from pass
  juce::String settingsInfo;
  for (int j = 0; j < pass.settings.size(); ++j) {
    juce::String knobName = pass.settings.getName(j).toString();
    float val = (float)pass.settings.getValueAt(j);

    for (auto *slot : mappingSlots) {
      if (slot->nameEditor.getText() == knobName &&
          slot->mappedParamIndex >= 0) {
        hostManager.setParameterValue(slot->mappedParamIndex, val);
        slot->testSlider.setValue(val, juce::dontSendNotification);
        settingsInfo += knobName + "=" + juce::String(val, 2) + " ";
      }
    }
  }

  statusText = "Dry Run [" + juce::String(dryRunPassIndex + 1) + "/" +
               juce::String(batchPasses.size()) + "]: " + pass.filename +
               " -> " + settingsInfo;
  dryRunPassIndex++;
  repaint();
}

void MainComponent::mapSelectedToSlot(int slotIndex) {
  int paramIndex = parameterList.getSelectedRow();
  if (paramIndex >= 0) {
    auto *slot = mappingSlots[slotIndex];
    slot->mappedParamIndex = paramIndex;
    slot->mappedParamName = hostManager.getParameterName(paramIndex);
    slot->testSlider.setValue(hostManager.getParameterValue(paramIndex),
                              juce::dontSendNotification);
    slot->repaint();
  }
}

void MainComponent::updateParameterList() {
  parameterList.updateContent();
  parameterList.repaint();
}

void MainComponent::paint(juce::Graphics &g) {
  g.fillAll(
      getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
  auto area = getLocalBounds().removeFromBottom(30).reduced(20, 0);
  g.setColour(juce::Colours::white.withAlpha(0.6f));
  g.setFont(11.0f);
  g.drawText(statusText, area, juce::Justification::centredRight);
}

void MainComponent::resized() {
  auto area = getLocalBounds().reduced(20);
  titleLabel.setBounds(area.removeFromTop(40));

  auto topRow = area.removeFromTop(50);
  loadPluginButton.setBounds(
      topRow.removeFromLeft(160).withSizeKeepingCentre(150, 40));
  showGUIButton.setBounds(
      topRow.removeFromLeft(160).withSizeKeepingCentre(150, 40));
  importTrainerButton.setBounds(
      topRow.removeFromLeft(160).withSizeKeepingCentre(150, 40));
  dryRunButton.setBounds(
      topRow.removeFromLeft(160).withSizeKeepingCentre(150, 40));
  startCaptureButton.setBounds(
      topRow.removeFromRight(160).withSizeKeepingCentre(150, 40));

  area.removeFromTop(10);
  progressBar.setBounds(area.removeFromBottom(20));
  area.removeFromBottom(10);

  auto mainContent = area;
  auto leftPanel =
      mainContent.removeFromLeft(area.getWidth() * 0.7f).reduced(5);
  auto rightPanel = mainContent.reduced(5);

  auto configArea = leftPanel.removeFromTop(40);
  timeLabel.setBounds(configArea.removeFromLeft(80));
  timeSelection.setBounds(
      configArea.removeFromLeft(110).withSizeKeepingCentre(100, 25));
  slotCountLabel.setBounds(configArea.removeFromLeft(80));
  slotCountSelection.setBounds(
      configArea.removeFromLeft(70).withSizeKeepingCentre(60, 25));

  leftPanel.removeFromTop(10);
  slotViewport.setBounds(leftPanel);

  slotContainer.setBounds(0, 0, slotViewport.getWidth() - 20,
                          mappingSlots.size() * 95);
  int y = 0;
  for (auto *slot : mappingSlots) {
    slot->setBounds(0, y, slotContainer.getWidth(), 90);
    y += 95;
  }

  parameterList.setBounds(rightPanel);
}
