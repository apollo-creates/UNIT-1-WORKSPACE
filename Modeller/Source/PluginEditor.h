#pragma once

#include "PluginProcessor.h"
#include <juce_audio_processors/juce_audio_processors.h>
#include <memory>

//==============================================================================
class UNIT1ModellerAudioProcessorEditor : public juce::AudioProcessorEditor {
public:
  UNIT1ModellerAudioProcessorEditor(UNIT1ModellerAudioProcessor &);
  ~UNIT1ModellerAudioProcessorEditor() override;

  void paint(juce::Graphics &) override;
  void resized() override;

private:
  UNIT1ModellerAudioProcessor &audioProcessor;

  // --- FIGMA V2 LOOK AND FEEL ---
  struct CustomLookAndFeel : public juce::LookAndFeel_V4 {
    void drawRotarySlider(juce::Graphics &g, int x, int y, int width,
                          int height, float sliderPos,
                          const float rotaryStartAngle,
                          const float rotaryEndAngle,
                          juce::Slider &slider) override {
      auto radius = (float)juce::jmin(width, height) / 2.0f;
      auto centreX = (float)x + (float)width * 0.5f;
      auto centreY = (float)y + (float)height * 0.5f;
      auto rx = centreX - radius;
      auto ry = centreY - radius;
      auto rw = radius * 2.0f;
      auto angle =
          rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

      // Outer Circle (#4d4d4d body)
      g.setColour(juce::Colour(0xff4d4d4d));
      g.fillEllipse(rx, ry, rw, rw);
      g.setColour(juce::Colour(0xffababab));
      g.drawEllipse(rx, ry, rw, rw, 0.5f);

      // Inner Circle (53x53)
      auto innerRadius = 26.5f;
      auto innerR = innerRadius * 2.0f;
      auto innerRx = centreX - innerRadius;
      auto innerRy = centreY - innerRadius;

      juce::DropShadow(juce::Colour(0x21000000), 6, {0, 4})
          .drawForRectangle(
              g, juce::Rectangle<float>(innerRx, innerRy, innerR, innerR)
                     .getSmallestIntegerContainer());

      g.setColour(juce::Colour(0xff4d4d4d));
      g.fillEllipse(innerRx, innerRy, innerR, innerR);
      g.setColour(juce::Colour(0xffc8c8c8));
      g.drawEllipse(innerRx, innerRy, innerR, innerR, 1.0f);

      // Indicator Line
      juce::Path p;
      p.addLineSegment(
          juce::Line<float>(centreX, centreY - 18.0f, centreX, centreY - 4.0f),
          2.0f);
      p.applyTransform(
          juce::AffineTransform::rotation(angle, centreX, centreY));
      g.setColour(juce::Colour(0xffe2e2e2));
      juce::DropShadow(juce::Colour(0x40000000), 4, {0, 4}).drawForPath(g, p);
      g.strokePath(p, juce::PathStrokeType(2.0f));
    }

    void drawLinearSlider(juce::Graphics &g, int x, int y, int width,
                          int height, float sliderPos, float minSliderPos,
                          float maxSliderPos,
                          const juce::Slider::SliderStyle style,
                          juce::Slider &slider) override {
      if (slider.getName() == "IR_MIX") {
        // Custom AB Switch Rail & Handle
        auto b = slider.getLocalBounds().toFloat();

        // Rail
        g.setColour(juce::Colour(0xff202020));
        g.fillRoundedRectangle(0, b.getHeight() * 0.5f - 3.0f, b.getWidth(),
                               6.0f, 5.0f);

        // Handle
        float handleX = sliderPos - 7.5f;
        g.setColour(juce::Colour(0xff232323));
        g.fillRoundedRectangle(handleX, 0, 15.0f, 34.0f, 5.0f);
        g.setColour(juce::Colour(0xff1f1f1f));
        g.drawRoundedRectangle(handleX, 0, 15.0f, 34.0f, 5.0f, 1.0f);

        // Cap Notch
        g.setColour(juce::Colour(0xffd9d9d9));
        g.fillRect(handleX + 7.0f, 3.0f, 1.0f, 28.0f);
        return;
      }
      LookAndFeel_V4::drawLinearSlider(g, x, y, width, height, sliderPos,
                                       minSliderPos, maxSliderPos, style,
                                       slider);
    }
  };

  CustomLookAndFeel customLookAndFeel;

  // --- FIGMA V2 COMPONENTS ---
  struct Speaker : public juce::Component {
    void paint(juce::Graphics &g) override {
      auto b = getLocalBounds().toFloat();
      juce::Path hp;
      hp.addEllipse(b);
      juce::DropShadow(juce::Colour(0x40000000), 4.0f, {0, 4})
          .drawForPath(g, hp);
      g.setColour(juce::Colour(0xff202020));
      g.fillEllipse(b);

      auto inner = b.reduced(8.0f);
      juce::Path cp;
      cp.addEllipse(inner);
      juce::DropShadow(juce::Colour(0xab000000), 6.5f, {0, 1})
          .drawForPath(g, cp);
      g.setColour(juce::Colour(0xff000000));
      g.fillEllipse(inner);
      g.setColour(juce::Colour(0xff565656));
      g.drawEllipse(inner, 1.0f);

      auto dust = inner.withSizeKeepingCentre(72, 69);
      juce::Path dp;
      dp.addEllipse(dust);
      juce::DropShadow(juce::Colour(0x40000000), 4.0f, {0, 4})
          .drawForPath(g, dp);
      g.setColour(juce::Colour(0xff444444));
      g.fillEllipse(dust);
      g.setColour(juce::Colour(0xff090909));
      g.drawEllipse(dust, 3.0f);
    }
  };

  struct PedalPanel : public juce::Component {
    void paint(juce::Graphics &g) override {
      auto b = getLocalBounds().toFloat();
      juce::Path p;
      p.addRoundedRectangle(b, 25.0f);
      juce::DropShadow(juce::Colour(0x3b000000), 8.3f, {0, 4})
          .drawForPath(g, p);
      juce::DropShadow(juce::Colour(0x40000000), 4.0f, {0, 4})
          .drawForPath(g, p);
      g.setColour(juce::Colour(0xfff1f1f1));
      g.fillPath(p);
      g.setColour(juce::Colour(0xffdadada));
      g.strokePath(p, juce::PathStrokeType(4.0f));

      // Switch
      auto swB = juce::Rectangle<float>(79.0f, 297.0f, 70.0f, 70.0f);
      g.setColour(juce::Colour(0xff4d4d4d));
      g.drawEllipse(swB, 5.0f);
      auto swI = swB.reduced(10.0f);
      g.setColour(juce::Colour(0xff6c6c6c));
      g.fillEllipse(swI);
      g.setColour(juce::Colour(0xff6f6f6f));
      g.drawEllipse(swI, 2.0f);
    }
  };

  struct AmpPanel : public juce::Component {
    void paint(juce::Graphics &g) override {
      auto b = getLocalBounds().toFloat();
      juce::Path body;
      body.addRoundedRectangle(b, 13.0f);
      juce::DropShadow(juce::Colour(0x3b000000), 8.3f, {0, 4})
          .drawForPath(g, body);
      g.setColour(juce::Colour(0xff3f3f3f));
      g.strokePath(body, juce::PathStrokeType(4.0f));

      // Grill
      auto grill = juce::Rectangle<float>(13.0f, 13.0f, 800.0f, 235.0f);
      juce::Path gp;
      gp.addRoundedRectangle(grill.getX(), grill.getY(), grill.getWidth(),
                             grill.getHeight(), 8.0f, 8.0f, true, true, false,
                             false);
      juce::ColourGradient grad(juce::Colour(0xff000000), 0, 13,
                                juce::Colour(0xff424242), 0, 248, false);
      g.setGradientFill(grad);
      g.fillPath(gp);

      // Dot Pattern Mesh
      g.setColour(juce::Colour(35, 35, 38).withAlpha(0.6f));
      for (float dy = 20; dy < 240; dy += 6) {
        for (float dx = 20; dx < 790; dx += 6) {
          g.fillEllipse(dx, dy, 2, 2);
        }
      }

      // Control Background
      auto ctrl = juce::Rectangle<float>(13.0f, 248.0f, 800.0f, 135.0f);
      g.setColour(juce::Colour(0xffe9e9e9));
      juce::Path cp;
      cp.addRoundedRectangle(ctrl.getX(), ctrl.getY(), ctrl.getWidth(),
                             ctrl.getHeight(), 5.0f, 5.0f, false, false, true,
                             true);
      g.fillPath(cp);
    }
  };

  struct LoadBox : public juce::Component {
    LoadBox(const juce::String &txt) {
      l.setText(txt, juce::dontSendNotification);
      l.setJustificationType(juce::Justification::centred);
      l.setColour(juce::Label::textColourId, juce::Colour(0xff606060));
      l.setFont(juce::FontOptions("Manrope", 11.0f, 0));
      addAndMakeVisible(l);
      addAndMakeVisible(b);
      b.setAlpha(0.0f);
    }
    void paint(juce::Graphics &g) override {
      auto r = getLocalBounds().toFloat();
      juce::DropShadow(juce::Colour(0x1e000000), 7, {0, 4})
          .drawForRectangle(g, r.getSmallestIntegerContainer());
      g.setColour(juce::Colour(0xff343333));
      g.fillRoundedRectangle(r, 5.0f);
      g.setColour(juce::Colour(0xff3f3f3f));
      g.drawRoundedRectangle(r, 5.0f, 1.0f);
    }
    void resized() override {
      l.setBounds(getLocalBounds());
      b.setBounds(getLocalBounds());
    }
    void setText(const juce::String &t) {
      l.setText(t, juce::dontSendNotification);
    }
    juce::Label l;
    juce::TextButton b;
  };

  // UI ELEMENTS
  juce::Slider pedalKnobs[4], ampKnobs[9], irMixSlider;
  PedalPanel pedalPanel;
  AmpPanel ampPanel;
  Speaker speaker1, speaker2;
  LoadBox bDrive, bAmp, bIR1, bIR2;
  juce::Label lAlpha, lMusic, lCab, lUnit1, lDrive, lAmp;

  void handleLoadPedal();
  void handleLoadAmp();
  void handleLoadIRA();
  void handleLoadIRB();
  std::unique_ptr<juce::FileChooser> chooser;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(
      UNIT1ModellerAudioProcessorEditor)
};
