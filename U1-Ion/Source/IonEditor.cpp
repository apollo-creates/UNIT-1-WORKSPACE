#include "IonEditor.h"
#include "BinaryData.h"

void IonEditor::IonLookAndFeel::drawRotarySlider(
    juce::Graphics &g, int x, int y, int w, int h, float sliderPos,
    float startAngle, float endAngle, juce::Slider &slider) {

  auto angle = startAngle + sliderPos * (endAngle - startAngle);
  auto bounds = juce::Rectangle<int>(x, y, w, h).toFloat().reduced(2.0f);
  auto cx = bounds.getCentreX();
  auto cy = bounds.getCentreY();
  auto r = juce::jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f;

  if (knobImage.isValid()) {
    auto imgScale = (r * 2.0f) / (float)knobImage.getWidth();
    juce::AffineTransform t =
        juce::AffineTransform::rotation(angle, cx, cy)
            .followedBy(juce::AffineTransform::scale(imgScale).translated(
                cx - r, cy - r));
    g.drawImageTransformed(knobImage, t);
  } else {
    g.setColour(juce::Colours::white);
    g.drawEllipse(bounds, 2.0f);
    g.drawLine(cx, cy, cx + std::sin(angle) * r, cy - std::cos(angle) * r,
               2.0f);
  }
}

IonEditor::IonEditor(IonProcessor &p)
    : AudioProcessorEditor(&p), proc(p), powerButton("POWER"),
      settingsButton("SETTINGS"), powerAmpModeBtn("POWER AMP") {

  setLookAndFeel(&ionLAF);

  bgOn = juce::ImageCache::getFromMemory(
      BinaryData::
          ION__UI_v1_2026Feb12_114941PM000_CustomizedView15444265983_png,
      BinaryData::
          ION__UI_v1_2026Feb12_114941PM000_CustomizedView15444265983_pngSize);
  bgOff = juce::ImageCache::getFromMemory(
      BinaryData::Main_Backgroud_Power_Off_PNG,
      BinaryData::Main_Backgroud_Power_Off_PNGSize);
  ionLAF.knobImage = juce::ImageCache::getFromMemory(
      BinaryData::Knob_v2_png, BinaryData::Knob_v2_pngSize);

  addAndMakeVisible(powerButton);
  powerButton.setClickingTogglesState(true);
  powerButton.setToggleState(proc.isPowerOn(), juce::dontSendNotification);
  powerButton.onClick = [this] {
    proc.setPowerOn(powerButton.getToggleState());
  };

  addAndMakeVisible(settingsButton);
  settingsButton.onClick = [this] {
    auto chooser = std::make_unique<juce::FileChooser>("Select Amp Model",
                                                       juce::File{}, "*.json");
    chooser->launchAsync(juce::FileBrowserComponent::openMode |
                             juce::FileBrowserComponent::canSelectFiles,
                         [this](const juce::FileChooser &fc) {
                           if (fc.getResult().exists())
                             proc.loadAmpModel(fc.getResult());
                         });
  };

  addAndMakeVisible(powerAmpModeBtn);
  powerAmpModeBtn.setClickingTogglesState(true);
  powerAmpModeBtn.setColour(juce::TextButton::buttonOnColourId,
                            juce::Colours::orange);
  powerAmpModeBtn.onClick = [this] {
    powerAmpMode = powerAmpModeBtn.getToggleState();
    resized();
  };

  // Setup Knobs
  for (int i = 0; i < 8; ++i)
    setupKnob(ampKnobs[i], ampLabels[i], "");
  for (int i = 0; i < 4; ++i)
    setupKnob(pedalKnobs[i], pedalLabels[i], "");
  setupKnob(inputKnob, inputLabel, "IN");
  setupKnob(outputKnob, outputLabel, "OUT");

  // Nav
  setupNav(navDrive, "DRIVE", View::Drive);
  setupNav(navAmp, "AMP", View::Amp);
  setupNav(navCab, "CAB", View::Cab);

  // Cab UI
  addAndMakeVisible(cabA);
  addAndMakeVisible(cabB);

  auto setupPanel = [this](CabPanel &cp, bool isA) {
    cp.addAndMakeVisible(cp.loadBtn);
    cp.loadBtn.setButtonText("LOAD IR");
    cp.loadBtn.onClick = [this, &cp, isA] {
      chooser = std::make_unique<juce::FileChooser>("Select IR", juce::File{},
                                                    "*.wav");
      chooser->launchAsync(juce::FileBrowserComponent::openMode |
                               juce::FileBrowserComponent::canSelectFiles,
                           [this, &cp, isA](const juce::FileChooser &fc) {
                             if (fc.getResult().exists()) {
                               if (isA)
                                 proc.loadCabinetIRA(fc.getResult());
                               else
                                 proc.loadCabinetIRB(fc.getResult());
                               cp.fileLabel.setText(
                                   fc.getResult().getFileName(),
                                   juce::dontSendNotification);
                             }
                           });
    };
    cp.addAndMakeVisible(cp.fileLabel);
    cp.fileLabel.setText("- NO IR -", juce::dontSendNotification);
    cp.fileLabel.setJustificationType(juce::Justification::centred);

    // Sliders
    auto setupS = [&](juce::Slider &s, const juce::String &l) {
      s.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
      s.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
      cp.addAndMakeVisible(s);
    };
    setupS(cp.lowCut, "LO");
    setupS(cp.hiCut, "HI");
    setupS(cp.level, "LVL");
  };
  setupPanel(cabA, true);
  setupPanel(cabB, false);

  addAndMakeVisible(abMix);
  abMix.setRange(0.0, 1.0, 0.01);
  abMix.setValue(0.5);
  abMix.onValueChange = [this] {
    proc.getCab().setMix((float)abMix.getValue());
  };

  addAndMakeVisible(abMixLabel);
  abMixLabel.setText("MIX", juce::dontSendNotification);
  abMixLabel.setJustificationType(juce::Justification::centred);

  startTimerHz(30);
  setSize(1280, 620);
}

IonEditor::~IonEditor() {
  stopTimer();
  setLookAndFeel(nullptr);
}

void IonEditor::timerCallback() {
  inputMeter = proc.getInputMeterLevel();
  outputMeter = proc.getOutputMeterLevel();
  repaint();
}

void IonEditor::paint(juce::Graphics &g) {
  g.drawImageAt(powerButton.getToggleState() ? bgOn : bgOff, 0, 48);
  paintMeter(g, juce::Rectangle<int>(12, 185, 12, 100), inputMeter);
  paintMeter(g, juce::Rectangle<int>(1256, 185, 12, 100), outputMeter);
}

void IonEditor::paintMeter(juce::Graphics &g, juce::Rectangle<int> area,
                           float level) {
  g.setColour(juce::Colour(0xff1a1a1a));
  g.fillRect(area);
  g.setColour(juce::Colour(0xff00ff00).withAlpha(0.6f));
  int h = (int)(area.getHeight() * level);
  g.fillRect(area.getX(), area.getBottom() - h, area.getWidth(), h);
}

void IonEditor::resized() {
  powerButton.setBounds(1178, 6, 36, 36);
  settingsButton.setBounds(1226, 6, 36, 36);
  powerAmpModeBtn.setBounds(1000, 6, 120, 36);

  navDrive.setBounds(98, 14, 70, 24);
  navAmp.setBounds(178, 14, 70, 24);
  navCab.setBounds(258, 14, 70, 24);

  // Global Gains (Level 5.0 is Unity/1.0)
  inputKnob.setBounds(40, 190, 80, 80);
  inputLabel.setBounds(40, 270, 80, 20);
  inputKnob.onValueChange = [this] {
    proc.setInputLevel((float)inputKnob.getValue() / 5.0f);
  };

  outputKnob.setBounds(1160, 190, 80, 80);
  outputLabel.setBounds(1160, 270, 80, 20);
  outputKnob.onValueChange = [this] {
    proc.setOutputLevel((float)outputKnob.getValue() / 5.0f);
  };

  // Amp Knobs
  bool isAmp = (currentView == View::Amp);
  int ampX = 340, ampY = 190, ampGap = 63, ampSz = 60;

  if (isAmp) {
    if (!powerAmpMode) {
      const char *names[] = {"PRE GAIN",  "LOW", "MID", "HIGH",
                             "POST GAIN", "",    "",    ""};
      for (int i = 0; i < 8; ++i) {
        ampKnobs[i].setVisible(names[i][0] != '\0');
        ampLabels[i].setVisible(names[i][0] != '\0');
        ampLabels[i].setText(names[i], juce::dontSendNotification);
      }
      ampKnobs[0].onValueChange = [this] {
        proc.getAmp().setPreGain((float)ampKnobs[0].getValue());
      };
      ampKnobs[1].onValueChange = [this] {
        proc.getAmp().setBass((float)ampKnobs[1].getValue());
      };
      ampKnobs[2].onValueChange = [this] {
        proc.getAmp().setMid((float)ampKnobs[2].getValue());
      };
      ampKnobs[3].onValueChange = [this] {
        proc.getAmp().setTreb((float)ampKnobs[3].getValue());
      };
      ampKnobs[4].onValueChange = [this] {
        proc.getAmp().setPostGain((float)ampKnobs[4].getValue());
      };
    } else {
      const char *names[] = {"PRESENCE", "DEPTH", "SAG", "BIAS",
                             "BIAS X",   "",      "",    ""};
      for (int i = 0; i < 8; ++i) {
        ampKnobs[i].setVisible(names[i][0] != '\0');
        ampLabels[i].setVisible(names[i][0] != '\0');
        ampLabels[i].setText(names[i], juce::dontSendNotification);
      }
      ampKnobs[0].onValueChange = [this] {
        proc.getAmp().setPresence((float)ampKnobs[0].getValue());
      };
      ampKnobs[1].onValueChange = [this] {
        proc.getAmp().setDepth((float)ampKnobs[1].getValue());
      };
      ampKnobs[2].onValueChange = [this] {
        proc.getAmp().setSag((float)ampKnobs[2].getValue());
      };
      ampKnobs[3].onValueChange = [this] {
        proc.getAmp().setBias((float)ampKnobs[3].getValue());
      };
      ampKnobs[4].onValueChange = [this] {
        proc.getAmp().setBiasX((float)ampKnobs[4].getValue());
      };
    }
  }

  for (int i = 0; i < 8; ++i) {
    if (!isAmp) {
      ampKnobs[i].setVisible(false);
      ampLabels[i].setVisible(false);
    }
    ampKnobs[i].setBounds(ampX + (i * ampGap), ampY, ampSz, ampSz);
    ampLabels[i].setBounds(ampX + (i * ampGap), ampY + ampSz, ampSz, 20);
  }

  // Pedal
  bool isPedal = (currentView == View::Drive);
  const char *pedNames[] = {"DRIVE", "ATTACK", "BRIGHT", "LEVEL"};
  for (int i = 0; i < 4; ++i) {
    pedalKnobs[i].setVisible(isPedal);
    pedalLabels[i].setVisible(isPedal);
    pedalLabels[i].setText(pedNames[i], juce::dontSendNotification);
    pedalKnobs[i].setBounds(450 + (i * 100), 250, 80, 80);
    pedalLabels[i].setBounds(450 + (i * 100), 330, 80, 20);
    pedalKnobs[0].onValueChange = [this] {
      proc.getPedal().setDrive((float)pedalKnobs[0].getValue());
    };
    pedalKnobs[1].onValueChange = [this] {
      proc.getPedal().setAttack((float)pedalKnobs[1].getValue());
    };
    pedalKnobs[2].onValueChange = [this] {
      proc.getPedal().setBright((float)pedalKnobs[2].getValue());
    };
    pedalKnobs[3].onValueChange = [this] {
      proc.getPedal().setLevel((float)pedalKnobs[3].getValue());
    };
  }

  // Cab
  bool isCab = (currentView == View::Cab);
  cabA.setVisible(isCab);
  cabB.setVisible(isCab);
  abMix.setVisible(isCab);
  abMixLabel.setVisible(isCab);
  cabA.setBounds(100, 100, 400, 400);
  cabB.setBounds(780, 100, 400, 400);

  auto layoutCab = [](CabPanel &cp) {
    cp.loadBtn.setBounds(100, 50, 200, 40);
    cp.fileLabel.setBounds(20, 100, 360, 30);
    cp.lowCut.setBounds(50, 150, 80, 80);
    cp.hiCut.setBounds(160, 150, 80, 80);
    cp.level.setBounds(270, 150, 80, 80);
  };
  layoutCab(cabA);
  layoutCab(cabB);

  abMix.setBounds(540, 350, 200, 20);
  abMixLabel.setBounds(540, 330, 200, 20);
}

void IonEditor::setupKnob(juce::Slider &s, juce::Label &l,
                          const juce::String &text) {
  s.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
  s.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
  s.setRange(0.0, 10.0, 0.1);
  s.setValue(5.0);
  addAndMakeVisible(s);
  l.setText(text, juce::dontSendNotification);
  l.setJustificationType(juce::Justification::centred);
  addAndMakeVisible(l);
}

void IonEditor::setupNav(juce::TextButton &b, const juce::String &text,
                         View v) {
  b.setButtonText(text);
  b.onClick = [this, v] {
    currentView = v;
    resized();
  };
  addAndMakeVisible(b);
}
