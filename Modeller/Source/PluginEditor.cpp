#include "PluginEditor.h"
#include "PluginProcessor.h"

UNIT1ModellerAudioProcessorEditor::UNIT1ModellerAudioProcessorEditor(
    UNIT1ModellerAudioProcessor &p)
    : AudioProcessorEditor(&p), audioProcessor(p), bDrive("DRIVE LOAD"),
      bAmp("AMP LOAD"), bIR1("IR LOAD"), bIR2("IR LOAD") {

  setLookAndFeel(&customLookAndFeel);

  // Structural Backdrops
  addAndMakeVisible(pedalPanel);
  addAndMakeVisible(ampPanel);

  // Knobs
  auto setupKnob = [](juce::Slider &s) {
    s.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    s.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    s.setRange(0.0, 10.0, 0.1);
    s.setValue(5.0);
  };

  for (int i = 0; i < 4; ++i) {
    addAndMakeVisible(pedalKnobs[i]);
    setupKnob(pedalKnobs[i]);
  }
  for (int i = 0; i < 9; ++i) {
    addAndMakeVisible(ampKnobs[i]);
    setupKnob(ampKnobs[i]);
  }

  // Load Boxes
  addAndMakeVisible(bDrive);
  addAndMakeVisible(bAmp);
  addAndMakeVisible(bIR1);
  addAndMakeVisible(bIR2);

  // Speakers
  addAndMakeVisible(speaker1);
  addAndMakeVisible(speaker2);

  // IR Slider
  addAndMakeVisible(irMixSlider);
  irMixSlider.setName("IR_MIX");
  irMixSlider.setSliderStyle(juce::Slider::LinearHorizontal);
  irMixSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
  irMixSlider.setRange(0.0, 1.0, 0.01);
  irMixSlider.setValue(0.5);

  // Labels
  auto setupLabel = [this](juce::Label &l, const juce::String &t, float sz,
                           juce::Colour c) {
    l.setText(t, juce::dontSendNotification);
    l.setFont(juce::FontOptions("Manrope", sz, 0));
    l.setColour(juce::Label::textColourId, c);
    addAndMakeVisible(l);
  };

  setupLabel(lUnit1, "UNIT 1 - MODELLER", 33.0f, juce::Colour(0xff616161));
  setupLabel(lAlpha, "ALPHA", 32.0f, juce::Colour(0xff616161));
  setupLabel(lMusic, "Music\nTechnology", 12.0f, juce::Colour(0xff060606));
  setupLabel(lCab, "CAB", 33.0f, juce::Colour(0xffffffff));
  setupLabel(lDrive, "DRIVE", 24.0f, juce::Colour(0xff000000));
  setupLabel(lAmp, "AMP", 48.0f, juce::Colour(0xffd6d6d6));

  // Callbacks
  bDrive.b.onClick = [this] { handleLoadPedal(); };
  bAmp.b.onClick = [this] { handleLoadAmp(); };
  bIR1.b.onClick = [this] { handleLoadIRA(); };
  bIR2.b.onClick = [this] { handleLoadIRB(); };

  irMixSlider.onValueChange = [this] {
    audioProcessor.getCab().setMix((float)irMixSlider.getValue());
  };

  // DSP
  pedalKnobs[0].onValueChange = [this] {
    audioProcessor.getPedal().setDrive((float)pedalKnobs[0].getValue());
  };
  pedalKnobs[1].onValueChange = [this] {
    audioProcessor.getPedal().setLevel((float)pedalKnobs[1].getValue());
  };
  pedalKnobs[2].onValueChange = [this] {
    audioProcessor.getPedal().setAttack((float)pedalKnobs[2].getValue());
  };
  pedalKnobs[3].onValueChange = [this] {
    audioProcessor.getPedal().setBright((float)pedalKnobs[3].getValue());
  };

  ampKnobs[0].onValueChange = [this] {
    audioProcessor.getAmp().setGain((float)ampKnobs[0].getValue());
  };
  ampKnobs[1].onValueChange = [this] {
    audioProcessor.getAmp().setLow((float)ampKnobs[1].getValue());
  };
  ampKnobs[2].onValueChange = [this] {
    audioProcessor.getAmp().setMid((float)ampKnobs[2].getValue());
  };
  ampKnobs[3].onValueChange = [this] {
    audioProcessor.getAmp().setHigh((float)ampKnobs[3].getValue());
  };
  ampKnobs[4].onValueChange = [this] {
    audioProcessor.getAmp().setLevel((float)ampKnobs[4].getValue());
  };

  setSize(1545, 800);
}

UNIT1ModellerAudioProcessorEditor::~UNIT1ModellerAudioProcessorEditor() {
  setLookAndFeel(nullptr);
}

void UNIT1ModellerAudioProcessorEditor::paint(juce::Graphics &g) {
  g.fillAll(juce::Colour(0xff292929));

  // Structural Framework
  g.setColour(juce::Colour(0xffb9b9b9));
  g.fillRect(0, 0, getWidth(), 57);
  g.fillRect(0, 755, getWidth(), 45);

  g.setColour(juce::Colour(0xff373737));
  g.fillRect(0, 57, 40, 697);
  g.fillRect(1505, 57, 40, 698);

  g.setColour(juce::Colour(0xff5b5b5b));
  g.drawLine(369, 64, 369, 744, 1.5f);
  g.drawLine(1257, 64, 1257, 743, 1.5f);

  // Accent Triangle
  g.setColour(juce::Colour(0xff000000));
  juce::Path p;
  p.addTriangle(469, 300, 485, 316, 469, 332);
  g.fillPath(p);
}

void UNIT1ModellerAudioProcessorEditor::resized() {
  lUnit1.setBounds(18, 6, 300, 45);
  lAlpha.setBounds(1362, 754, 97, 44);
  lMusic.setBounds(1465, 762, 75, 30);
  lCab.setBounds(1349, 66, 70, 45);

  pedalPanel.setBounds(90, 213, 229, 395);
  ampPanel.setBounds(404, 213, 825, 395);

  bDrive.setBounds(84, 135, 240, 30);
  bAmp.setBounds(634, 135, 365, 30);
  bIR1.setBounds(1281, 135, 200, 30);
  bIR2.setBounds(1281, 672, 200, 30);

  pedalKnobs[0].setBounds(109, 247, 70, 70);
  pedalKnobs[1].setBounds(229, 247, 70, 70);
  pedalKnobs[2].setBounds(109, 367, 70, 70);
  pedalKnobs[3].setBounds(229, 367, 70, 70);
  lDrive.setBounds(173, 326, 80, 40);

  for (int i = 0; i < 9; ++i)
    ampKnobs[i].setBounds(434 + (i * 87), 500, 70, 70);

  lAmp.setBounds(767 + 404, 315 + 213, 100,
                 70); // Offset lAmp relative to ampPanel center?
  // Wait, lAmp in the old code was 767, 315 relative to amp group.
  // Let's place it at global 767+404, 315+213 if it was inside the group.
  lAmp.setBounds(767 + 404 - 200, 315 + 213 - 100, 200, 100);
  // Better yet, just match the v2 export: 767, 315.
  lAmp.setBounds(767, 315, 99, 66);

  speaker1.setBounds(1288, 192, 186, 179);
  speaker2.setBounds(1288, 451, 186, 179);
  irMixSlider.setBounds(1301, 394, 160, 34);
}

void UNIT1ModellerAudioProcessorEditor::handleLoadPedal() {
  chooser =
      std::make_unique<juce::FileChooser>("Pedal", juce::File{}, "*.json");
  chooser->launchAsync(juce::FileBrowserComponent::openMode |
                           juce::FileBrowserComponent::canSelectFiles,
                       [this](const juce::FileChooser &fc) {
                         if (fc.getResult().exists()) {
                           audioProcessor.loadPedalModel(fc.getResult());
                           bDrive.setText(fc.getResult().getFileName());
                         }
                       });
}
void UNIT1ModellerAudioProcessorEditor::handleLoadAmp() {
  chooser = std::make_unique<juce::FileChooser>("Amp", juce::File{}, "*.json");
  chooser->launchAsync(juce::FileBrowserComponent::openMode |
                           juce::FileBrowserComponent::canSelectFiles,
                       [this](const juce::FileChooser &fc) {
                         if (fc.getResult().exists()) {
                           audioProcessor.loadAmpModel(fc.getResult());
                           bAmp.setText(fc.getResult().getFileName());
                         }
                       });
}
void UNIT1ModellerAudioProcessorEditor::handleLoadIRA() {
  chooser = std::make_unique<juce::FileChooser>("IR A", juce::File{}, "*.wav");
  chooser->launchAsync(juce::FileBrowserComponent::openMode |
                           juce::FileBrowserComponent::canSelectFiles,
                       [this](const juce::FileChooser &fc) {
                         if (fc.getResult().exists()) {
                           audioProcessor.loadCabinetIRA(fc.getResult());
                           bIR1.setText(fc.getResult().getFileName());
                         }
                       });
}
void UNIT1ModellerAudioProcessorEditor::handleLoadIRB() {
  chooser = std::make_unique<juce::FileChooser>("IR B", juce::File{}, "*.wav");
  chooser->launchAsync(juce::FileBrowserComponent::openMode |
                           juce::FileBrowserComponent::canSelectFiles,
                       [this](const juce::FileChooser &fc) {
                         if (fc.getResult().exists()) {
                           audioProcessor.loadCabinetIRB(fc.getResult());
                           bIR2.setText(fc.getResult().getFileName());
                         }
                       });
}
