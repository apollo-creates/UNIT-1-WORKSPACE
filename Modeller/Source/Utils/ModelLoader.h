#pragma once

#include "../DSP/HorizonDrive.h"
#include "../DSP/PVVitriol.h"
#include <juce_core/juce_core.h>

class ModelLoader {
public:
  static bool loadPedalWeights(const juce::File &file,
                               HorizonDrive & /*pedal*/) {
    auto json = juce::JSON::parse(file);
    if (json.isObject()) {
      // Extract the learned parameters here once the Python trainer export
      // format is finalized
      return true;
    }
    return false;
  }

  static bool loadAmpWeights(const juce::File &file, PVVitriolAmp & /*amp*/) {
    auto json = juce::JSON::parse(file);
    if (json.isObject()) {
      return true;
    }
    return false;
  }
};
