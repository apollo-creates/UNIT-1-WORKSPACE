cmake_minimum_required(VERSION 3.15)

project(UNIT1_MODELLER VERSION 1.0.0)

# 1. Fetch JUCE
include(FetchContent)
FetchContent_Declare(
    juce
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG        8.0.4 # Updated for macOS 15 support
)
FetchContent_MakeAvailable(juce)

# 2. Add Plugin
juce_add_plugin(UNIT1_MODELLER
    COMPANY_NAME "NeuralBridge"
    PLUGIN_NAME "UNIT 1 Modeller"
    PRODUCT_NAME "UNIT1Modeller"
    FORMATS VST3 AU Standalone
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS TRUE
    COPY_STEP TRUE
    PLUGIN_CODE "U1ML"
    PLUGIN_MANUFACTURER_CODE "NBRI"
)

# 3. Source Files
target_sources(UNIT1_MODELLER PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginProcessor.h
    Source/PluginEditor.cpp
    Source/PluginEditor.h
    Source/DSP/HorizonDrive.h
    Source/DSP/PVVitriol.h
    Source/UI/CustomLookAndFeel.h
    Source/Utils/ModelLoader.h
)

# 4. Libraries and Modules
target_link_libraries(UNIT1_MODELLER PRIVATE
    juce::juce_audio_processors
    juce::juce_audio_utils
    juce::juce_dsp
    juce::juce_gui_extra
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags
)

# 5. Resources (Binary Data)
juce_add_binary_data(UNIT1_RESOURCES
    SOURCES
    Source/UI/Knob.png
    Source/UI/Knob_Ring.png
    Source/UI/New_Background.png
    Source/UI/Pedal_Glow_Ring.png
    Source/UI/Pedal_Switch.png
)

target_link_libraries(UNIT1_MODELLER PRIVATE UNIT1_RESOURCES)

target_compile_definitions(UNIT1_MODELLER PRIVATE
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
)
