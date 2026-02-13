#pragma once

#include "BinaryData.h"
#include <juce_gui_basics/juce_gui_basics.h>

class CustomLookAndFeel : public juce::LookAndFeel_V4 {
public:
  CustomLookAndFeel() {
    knobImg = juce::ImageCache::getFromMemory(BinaryData::Knob_png,
                                              BinaryData::Knob_pngSize);
    ringImg = juce::ImageCache::getFromMemory(BinaryData::Knob_Ring_png,
                                              BinaryData::Knob_Ring_pngSize);
    switchImg = juce::ImageCache::getFromMemory(
        BinaryData::Pedal_Switch_png, BinaryData::Pedal_Switch_pngSize);
    glowImg = juce::ImageCache::getFromMemory(
        BinaryData::Pedal_Glow_Ring_png, BinaryData::Pedal_Glow_Ring_pngSize);
  }

  void drawRotarySlider(juce::Graphics &g, int x, int y, int width, int height,
                        float sliderPos, const float rotaryStartAngle,
                        const float rotaryEndAngle,
                        juce::Slider &slider) override {
    if (knobImg.isNull()) {
      juce::LookAndFeel_V4::drawRotarySlider(g, x, y, width, height, sliderPos,
                                             rotaryStartAngle, rotaryEndAngle,
                                             slider);
      return;
    }

    auto bounds = juce::Rectangle<int>(x, y, width, height).toFloat();
    auto radius = juce::jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f;
    auto centre = bounds.getCentre();

    // 1. Draw Knob (Rotating)
    auto angle =
        rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

    juce::AffineTransform t;
    float knobW = (float)knobImg.getWidth();
    float knobH = (float)knobImg.getHeight();

    // Match the ring diameter (approx 80-90% of bounds)
    float targetSize = radius * 2.0f;
    float scale = targetSize / knobW;

    t = t.rotated(angle, knobW / 2.0f, knobH / 2.0f);
    t = t.scaled(scale, scale);
    t = t.translated(centre.getX() - (targetSize / 2.0f),
                     centre.getY() - (targetSize / 2.0f));

    g.setOpacity(1.0f);
    g.drawImageTransformed(knobImg, t);
  }

  void drawButtonBackground(juce::Graphics &g, juce::Button &button,
                            const juce::Colour &backgroundColour,
                            bool isMouseOverButton,
                            bool isButtonDown) override {
    if (button.getName() == "BYPASS_STOMP") {
      if (button.getToggleState() && !glowImg.isNull())
        g.drawImageWithin(glowImg, 0, 0, button.getWidth(), button.getHeight(),
                          juce::RectanglePlacement::centred);

      if (!switchImg.isNull())
        g.drawImageWithin(switchImg, 0, 0, button.getWidth(),
                          button.getHeight(),
                          juce::RectanglePlacement::centred);
    } else {
      // Transparent buttons for the background "Load" boxes
      if (button.getButtonText().isEmpty() ||
          backgroundColour == juce::Colours::transparentBlack)
        return;

      juce::LookAndFeel_V4::drawButtonBackground(
          g, button, backgroundColour, isMouseOverButton, isButtonDown);
    }
  }

private:
  juce::Image knobImg, ringImg, switchImg, glowImg;
};
