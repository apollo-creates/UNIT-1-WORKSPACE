#pragma once

#include "IonProcessor.h"
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_extra/juce_gui_extra.h>

class IonEditor : public juce::AudioProcessorEditor, public juce::Timer {
public:
  IonEditor(IonProcessor &);
  ~IonEditor() override;

  void paint(juce::Graphics &) override;
  void resized() override;
  void timerCallback() override;

  enum class View { Amp, Drive, Cab };

private:
  IonProcessor &proc;
  View currentView = View::Amp;
  bool powerAmpMode = false;

  struct IonLookAndFeel : public juce::LookAndFeel_V4 {
    juce::Image knobImage;
    void drawRotarySlider(juce::Graphics &g, int x, int y, int w, int h,
                          float sliderPos, float startAngle, float endAngle,
                          juce::Slider &slider) override;
  };

  IonLookAndFeel ionLAF;
  juce::Image bgOn, bgOff;

  juce::TextButton powerButton;
  juce::TextButton settingsButton;
  juce::TextButton
      powerAmpModeBtn; // The specific button to toggle power amp tweaks

  // Shared Knobs (Dynamic Labels/Bindings)
  juce::Slider ampKnobs[8];
  juce::Label ampLabels[8];

  juce::Slider pedalKnobs[4];
  juce::Label pedalLabels[4];

  juce::Slider inputKnob, outputKnob;
  juce::Label inputLabel, outputLabel;

  // Meter levels
  float inputMeter = 0.0f, outputMeter = 0.0f;

  // Nav
  juce::TextButton navDrive, navAmp, navCab;

  // Cab UI
  struct CabPanel : public juce::Component {
    juce::TextButton loadBtn;
    juce::Label fileLabel;
    juce::Slider lowCut, hiCut, level;
  };
  CabPanel cabA, cabB;
  juce::Slider abMix;
  juce::Label abMixLabel;

  void setupKnob(juce::Slider &s, juce::Label &l, const juce::String &text);
  void setupNav(juce::TextButton &b, const juce::String &text, View v);
  void paintMeter(juce::Graphics &g, juce::Rectangle<int> area, float level);

  std::unique_ptr<juce::FileChooser> chooser;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IonEditor)
};
