#pragma once

#include "../DSP/HorizonDrive.h"
#include "../DSP/PVVitriol.h"
#include <juce_data_structures/juce_data_structures.h>

class ModelLoader {
public:
  static bool loadPedalWeights(const juce::File &file, HorizonDrive &pedal) {
    auto json = juce::JSON::parse(file);
    if (!json.isObject())
      return false;
    return applyPedalWeights(json, pedal);
  }

  static bool loadAmpWeights(const juce::File &file, PVVitriolAmp &amp) {
    auto json = juce::JSON::parse(file);
    if (!json.isObject())
      return false;
    return applyAmpWeights(json, amp);
  }

  static bool applyPedalWeights(const juce::var &json, HorizonDrive &pedal) {
    if (json.hasProperty("drive_a"))
      pedal.set_drive_a(json["drive_a"]);
    if (json.hasProperty("drive_b"))
      pedal.set_drive_b(json["drive_b"]);
    if (json.hasProperty("drive_c"))
      pedal.set_drive_c(json["drive_c"]);
    if (json.hasProperty("attack_a"))
      pedal.set_attack_a(json["attack_a"]);
    if (json.hasProperty("attack_b"))
      pedal.set_attack_b(json["attack_b"]);
    if (json.hasProperty("bright_a"))
      pedal.set_bright_a(json["bright_a"]);
    if (json.hasProperty("bright_b"))
      pedal.set_bright_b(json["bright_b"]);
    if (json.hasProperty("level_scaler"))
      pedal.set_level_scaler(json["level_scaler"]);
    return true;
  }

  static bool applyAmpWeights(const juce::var &json, PVVitriolAmp &amp) {
    if (json.hasProperty("pre_gain")) {
      auto pg = json["pre_gain"];
      amp.set_preg_a(pg["a"]);
      amp.set_preg_b(pg["b"]);
    }
    if (json.hasProperty("tone")) {
      auto t = json["tone"];
      amp.set_bass_a(t["bass"]);
      amp.set_mid_a(t["mid"]);
      amp.set_treb_a(t["treb"]);
    }
    if (json.hasProperty("power")) {
      auto p = json["power"];
      amp.set_master_a(p["master"]);
      amp.set_pres_a(p["presence"]);
      amp.set_depth_a(p["depth"]);
      amp.set_sag_a(p["sag"]);
      amp.set_bias_a(p["bias"]);
    }
    if (json.hasProperty("level")) {
      amp.set_level_scaler(json["level"]);
    }
    return true;
  }

  static bool loadPedalFromBinary(const void *data, int size,
                                  HorizonDrive &pedal) {
    auto json =
        juce::JSON::parse(juce::String::createStringFromData(data, size));
    if (!json.isObject())
      return false;
    return applyPedalWeights(json, pedal);
  }

  static bool loadAmpFromBinary(const void *data, int size, PVVitriolAmp &amp) {
    auto json =
        juce::JSON::parse(juce::String::createStringFromData(data, size));
    if (!json.isObject())
      return false;
    return applyAmpWeights(json, amp);
  }
};
