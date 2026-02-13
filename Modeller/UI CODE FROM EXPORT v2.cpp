```cpp
#pragma once

#include <JuceHeader.h>

    class Frame_1 : public juce::Component {
public:
  Frame_1() {
    setLookAndFeel(&customLookAndFeel);

    // Initialize Labels
    setupLabel(alphaLabel, "ALPHA", "Manrope", 32.0f, juce::Font::plain,
               juce::Colour(0xff616161));
    setupLabel(musicTechLabel, "Music\nTechnology", "Inter", 12.0f,
               juce::Font::plain, juce::Colour(0xff060606));
    setupLabel(cabLabel, "CAB", "Manrope", 33.0f, juce::Font::plain,
               juce::Colour(0xffffffff));
    setupLabel(unit1Label, "UNIT 1 - MODELLER", "Manrope", 33.0f,
               juce::Font::plain, juce::Colour(0xff616161));
    setupLabel(driveLabel, "DRIVE", "Manrope", 24.0f, juce::Font::plain,
               juce::Colour(0xff000000));
    setupLabel(ampLabel, "AMP", "Manrope", 48.0f, juce::Font::plain,
               juce::Colour(0xffd6d6d6));

    // Initialize Sliders (Knobs)
    // Pedal Knobs
    for (auto *slider : {&knob1, &knob2, &knob3, &knob4}) {
      addAndMakeVisible(slider);
      slider->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
      slider->setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
      slider->setRange(0.0, 1.0);
    }

    // Amp Knobs
    for (auto *slider : {&knob5, &knob6, &knob7, &knob8, &knob9, &knob10,
                         &knob11, &knob12, &knob13}) {
      addAndMakeVisible(slider);
      slider->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
      slider->setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
      slider->setRange(0.0, 1.0);
    }

    // Add Child Components
    addAndMakeVisible(speaker1);
    addAndMakeVisible(speaker2);

    setSize(1545, 800);
  }

  ~Frame_1() override { setLookAndFeel(nullptr); }

  void paint(juce::Graphics &g) override {
    g.fillAll(juce::Colour(0xff292929)); // Background

    // Main window background (Frame 1 is the container, Main Window group
    // covers 0,0) We painted the background, now let's draw structural
    // elements.

    // Header
    g.setColour(juce::Colour(0xffb9b9b9));
    g.fillRect(0, 0, getWidth(), 57);

    // Footer
    g.fillRect(0, 755, getWidth(), 45);

    // Left Meter Panel
    g.setColour(juce::Colour(0xff373737));
    g.fillRect(0, 57, 40, 697);

    // Right Meter Panel
    g.fillRect(1505, 57, 40, 698);

    // Divider 1
    g.setColour(juce::Colour(0xff5b5b5b));
    g.drawLine(juce::Line<float>(369.0f, 64.0f, 369.0f, 744.0f), 1.5f);

    // Divider 2
    g.drawLine(juce::Line<float>(1257.0f, 64.0f, 1257.0f, 743.0f), 1.5f);

    // Load Boxes (Rectangles)
    drawLoadBox(g, 84, 135, 240, 30);   // Drive Load Box
    drawLoadBox(g, 634, 135, 365, 30);  // Amp Load Box
    drawLoadBox(g, 1281, 135, 200, 30); // IR Load Box 1
    drawLoadBox(g, 1281, 672, 200, 30); // IR Load Box 2

    // Connector Rectangles between speakers
    // Rect 3
    juce::DropShadow connectorShadow(juce::Colours::black.withAlpha(0.25f), 4,
                                     {0, 4});
    {
      juce::Path p;
      p.addRoundedRectangle(1301.0f, 408.0f, 160.0f, 6.0f, 5.0f);
      // Inner shadow simulation via gradient or just solid for now, design says
      // Inner Shadow We'll stick to solid fill #202020
      g.setColour(juce::Colour(0xff202020));
      g.fillPath(p);
    }

    // Rect 4
    g.setColour(juce::Colour(0xff232323));
    g.fillRoundedRectangle(1373.0f, 394.0f, 15.0f, 34.0f, 5.0f);
    g.setColour(juce::Colour(0xff1f1f1f));
    g.drawRoundedRectangle(1373.0f, 394.0f, 15.0f, 34.0f, 5.0f, 1.0f);

    // Rect 5
    g.setColour(juce::Colour(0xffd9d9d9));
    g.fillRect(1380.0f, 397.0f, 1.0f, 28.0f);

    // Polygon 1 (Generic visual as points not specified, drawing a small
    // diamond shape at pos)
    g.setColour(juce::Colour(0xff000000));
    juce::Path polyPath;
    polyPath.addTriangle(469.0f, 300.0f, 485.0f, 316.0f, 469.0f, 332.0f);
    g.fillPath(polyPath);
  }

  void resized() override {
    // Text Positions
    alphaLabel.setBounds(1362, 754, 97, 44);
    musicTechLabel.setBounds(1465, 762, 65, 30);
    cabLabel.setBounds(1349, 66, 64, 45);
    unit1Label.setBounds(18, 6, 281, 45);

    // Pedal Group internal positions
    driveLabel.setBounds(173, 326, 63, 33);

    // Knobs
    knob1.setBounds(109, 247, 70, 70);
    knob2.setBounds(229, 247, 70, 70);
    knob3.setBounds(109, 367, 70, 70);
    knob4.setBounds(229, 367, 70, 70);

    // Amp Knobs
    int startX = 434;
    int gap = 87;
    knob6.setBounds(startX, 500, 70, 70);
    knob7.setBounds(startX + gap, 500, 70, 70);
    knob8.setBounds(startX + gap * 2, 500, 70, 70);
    knob9.setBounds(startX + gap * 3, 500, 70, 70);
    knob10.setBounds(startX + gap * 4, 500, 70, 70);
    knob11.setBounds(startX + gap * 5, 500, 70, 70);
    knob12.setBounds(startX + gap * 6, 500, 70, 70);
    knob13.setBounds(startX + gap * 7, 500, 70, 70);
    knob5.setBounds(startX + gap * 8, 500, 70, 70);

    ampLabel.setBounds(767, 315, 99, 66);

    // Speakers
    speaker1.setBounds(1288, 192, 186, 179);
    speaker2.setBounds(1288, 451, 186, 179);
  }

private:
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

      // Outer Circle (Ellipse 6)
      // Fill #4d4d4d, Stroke #ababab width 0.5
      g.setColour(juce::Colour(0xff4d4d4d));
      g.fillEllipse(rx, ry, rw, rw);
      g.setColour(juce::Colour(0xffababab));
      g.drawEllipse(rx, ry, rw, rw, 0.5f);

      // Inner Circle (Ellipse 5)
      // Size 53x53 centered. Radius is 35 (70/2). Inner is 26.5 (53/2).
      // Offset is 4.5 pixels.
      auto innerRadius = 26.5f;
      auto innerR = innerRadius * 2.0f;
      auto innerRx = centreX - innerRadius;
      auto innerRy = centreY - innerRadius;

      // Shadow for inner circle
      // Drop Shadow: offset(0, 4) blur=6.6 color=rgba(0,0,0,0.13)
      juce::DropShadow shadow(juce::Colour(0x21000000), 6.6f, {0, 4});
      shadow.drawForRectangle(
          g, juce::Rectangle<float>(innerRx, innerRy, innerR, innerR));

      g.setColour(juce::Colour(0xff4d4d4d));
      g.fillEllipse(innerRx, innerRy, innerR, innerR);
      g.setColour(juce::Colour(0xffc8c8c8));
      g.drawEllipse(innerRx, innerRy, innerR, innerR, 1.0f);

      // Indicator Line
      // Design: Line 1. Pos (145, 269) relative to Knob 1 group (109, 247).
      // Local center is 35, 35. Line center roughly 35, 22 (Upwards).
      // Size 13x0, Rotation 90 (Vertical).
      // We draw a line rotated by the slider angle.

      juce::Path p;
      auto lineLen = 13.0f;
      // Line offset from center. The Figma design shows it at the top (12
      // o'clock relative to rotation or static?) "Rotation: 90deg" on a 13x0
      // line usually means horizontal line rotated 90 -> vertical. If the knob
      // rotates, this line should rotate. Position (145, 269) in a (109,247)
      // box -> relative (36, 22). Center is (35,35). So it's slightly off
      // center X, and moved up Y. We will center it for rotary slider logic.

      p.addLineSegment(juce::Line<float>::fromStartAndAngle(
                           centreX, centreY - 18.0f, centreX, centreY - 4.0f),
                       2.0f);
      // Rotate path
      p.applyTransform(
          juce::AffineTransform::rotation(angle, centreX, centreY));

      g.setColour(juce::Colour(0xffe2e2e2));

      // Shadow for indicator
      juce::DropShadow lineShadow(juce::Colour(0x40000000), 4, {0, 4});
      lineShadow.drawForPath(g, p);

      g.strokePath(p, juce::PathStrokeType(2.0f));
    }
  };

  // Helper to draw Load Boxes
  void drawLoadBox(juce::Graphics &g, int x, int y, int w, int h) {
    juce::Path p;
    p.addRoundedRectangle((float)x, (float)y, (float)w, (float)h, 5.0f);

    juce::DropShadow ds(juce::Colour(0x1e000000), 7.7f, {0, 4});
    ds.drawForPath(g, p);

    g.setColour(juce::Colour(0xff343333));
    g.fillPath(p);
    g.setColour(juce::Colour(0xff3f3f3f));
    g.strokePath(p, juce::PathStrokeType(1.0f));
  }

  void setupLabel(juce::Label &label, const juce::String &text,
                  const juce::String &fontName, float fontSize, int style,
                  juce::Colour colour) {
    addAndMakeVisible(label);
    label.setText(text, juce::dontSendNotification);
    label.setFont(juce::Font(fontName, fontSize, style));
    label.setColour(juce::Label::textColourId, colour);
    label.setJustificationType(juce::Justification::topLeft);
  }

  // Custom Component for Speakers
  struct Speaker : public juce::Component {
    Speaker() = default;

    void paint(juce::Graphics &g) override {
      auto bounds = getLocalBounds().toFloat();

      // Ellipse 4 (Outer housing) #202020
      juce::DropShadow housingShadow(juce::Colour(0x40000000), 4.0f, {0, 4});
      housingShadow.drawForEllipse(g, bounds);

      g.setColour(juce::Colour(0xff202020));
      g.fillEllipse(bounds);

      // Ellipse 1 (Inner cone) #000000
      auto innerBounds =
          bounds.reduced(8.0f, 8.0f); // Approximation: (186-170)/2 = 8
      juce::DropShadow coneShadow(juce::Colour(0xab000000), 6.5f, {0, 1});
      coneShadow.drawForEllipse(g, innerBounds);

      g.setColour(juce::Colour(0xff000000));
      g.fillEllipse(innerBounds);
      g.setColour(juce::Colour(0xff565656));
      g.drawEllipse(innerBounds, 1.0f);

      // Ellipse 2 & 3 (Dustcap)
      // Ellipse 2 is Stroke #090909 w=3
      // Ellipse 3 is Fill #444444
      // Center X: (186-72)/2 = 57. Y: (179-69)/2 = 55.
      // Figma says Pos: (1345, 247) inside Frame 1. Relative to Speaker (1288,
      // 192) -> (57, 55).
      auto dustCapBounds =
          innerBounds.reduced((innerBounds.getWidth() - 72) / 2,
                              (innerBounds.getHeight() - 69) / 2);

      juce::DropShadow dustCapShadow(juce::Colour(0x40000000), 4.0f, {0, 4});
      dustCapShadow.drawForEllipse(g, dustCapBounds);

      g.setColour(juce::Colour(0xff444444));
      g.fillEllipse(dustCapBounds);
      g.setColour(juce::Colour(0xff090909));
      g.drawEllipse(dustCapBounds, 3.0f);
    }
  };

  // Custom Component for Pedal Body (Drawing the background and switch)
  struct PedalPanel : public juce::Component {
    PedalPanel() { setOpaque(false); }

    void paint(juce::Graphics &g) override {
      auto bounds = getLocalBounds().toFloat();

      // Pedal Body
      juce::Path p;
      p.addRoundedRectangle(bounds, 25.0f);

      juce::DropShadow ds1(juce::Colour(0x3b000000), 8.3f, {0, 4});
      juce::DropShadow ds2(juce::Colour(0x40000000), 4.0f, {0, 4});
      // Manually stacking shadows requires drawing twice or using an image,
      // simple approach: Draw with combined shadow or just draw twice with
      // bounds offset (messy). We will just use the stronger one.
      ds1.drawForPath(g, p);
      ds2.drawForPath(g, p);

      g.setColour(juce::Colour(0xfff1f1f1));
      g.fillPath(p);
      g.setColour(juce::Colour(0xffdadada));
      g.strokePath(p, juce::PathStrokeType(4.0f));

      // Pedal Glow Ring (Switch)
      // Position (169, 510) -> Relative (79, 297). Size 70x70.
      // Wait, design says Pedal Group is (90, 213). Glow Ring (169, 510).
      // That's relative to Frame 1.
      // Relative X = 169 - 90 = 79.
      // Relative Y = 510 - 213 = 297.
      // Pedal Height is 395. 297 + 35 = 332. Fits.

      auto switchBounds = juce::Rectangle<float>(79.0f, 297.0f, 70.0f, 70.0f);

      // Outer Glow Ring Stroke
      g.setColour(juce::Colour(0xff4d4d4d));
      g.drawEllipse(switchBounds, 5.0f);

      // Inner Glow Ring
      // Pos (179, 520) -> Relative (89, 307). Size 50x50.
      auto innerSwitchBounds =
          juce::Rectangle<float>(89.0f, 307.0f, 50.0f, 50.0f);
      g.setColour(juce::Colour(0xff6c6c6c));
      g.fillEllipse(innerSwitchBounds);
      g.setColour(juce::Colour(0xff6f6f6f));
      g.drawEllipse(innerSwitchBounds, 2.0f);
    }
  };

  // Custom Component for Amp Panel
  struct AmpPanel : public juce::Component {
    AmpPanel() { setOpaque(false); }

    void paint(juce::Graphics &g) override {
      auto bounds = getLocalBounds().toFloat();

      // Amp Body
      juce::Path bodyPath;
      bodyPath.addRoundedRectangle(bounds, 13.0f);

      juce::DropShadow ds1(juce::Colour(0x3b000000), 8.3f, {0, 4});
      juce::DropShadow ds2(juce::Colour(0x40000000), 4.0f, {0, 4});
      ds1.drawForPath(g, bodyPath);
      ds2.drawForPath(g, bodyPath);

      // Stroke
      g.setColour(juce::Colour(0xff3f3f3f));
      g.strokePath(bodyPath, juce::PathStrokeType(4.0f));

      // Grill Area
      // Rect (417, 226, 800, 235) relative to Frame 1.
      // Amp Group is (404, 213). Relative -> (13, 13).
      auto grillRect = juce::Rectangle<float>(13.0f, 13.0f, 800.0f, 235.0f);

      juce::Path grillPath;
      grillPath.addRoundedRectangle(grillRect, 8.0f, 8.0f, 0.0f, 0.0f);

      // Inner Shadow
      g.saveState();
      g.reduceClipRegion(grillPath);
      // To do inner shadow, we draw the shadow of a larger path behind the
      // current area? Standard hack: Draw shadow of the path on the outside,
      // inverted? Or simpler: Gradient + Border.

      // Fill Gradient (Simulate Fill[0] #000000 to Fill[1] #424242)
      juce::ColourGradient gradient(juce::Colour(0xff000000), 0.0f, 13.0f,
                                    juce::Colour(0xff424242), 0.0f,
                                    13.0f + 235.0f, false);
      g.setGradientFill(gradient);
      g.fillPath(grillPath);

      // Inner Shadow Approximation (Stroke)
      g.setColour(juce::Colour(0x3b000000));
      g.strokePath(grillPath, juce::PathStrokeType(4.0f)); // Dark border inside
      g.restoreState();

      // Rectangle 2 (Control Panel Background)
      // Rect (417, 461) -> Relative (13, 248). Size 800x135.
      auto controlRect = juce::Rectangle<float>(13.0f, 248.0f, 800.0f, 135.0f);
      g.setColour(juce::Colour(0xffe9e9e9));
      juce::Path controlPath;
      controlPath.addRoundedRectangle(controlRect, 0.0f, 0.0f, 5.0f, 5.0f);
      g.fillPath(controlPath);
    }
  };

  CustomLookAndFeel customLookAndFeel;

  // Text Labels
  juce::Label alphaLabel;
  juce::Label musicTechLabel;
  juce::Label cabLabel;
  juce::Label unit1Label;
  juce::Label driveLabel;
  juce::Label ampLabel;

  // Pedal Components
  juce::Slider knob1, knob2, knob3, knob4;
  PedalPanel pedalPanel; // We need to add this to the component tree to paint
                         // under knobs

  // Amp Components
  juce::Slider knob5, knob6, knob7, knob8, knob9, knob10, knob11, knob12,
      knob13;
  AmpPanel ampPanel; // We need to add this to the component tree

  // Speakers
  Speaker speaker1, speaker2;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Frame_1)
};
```