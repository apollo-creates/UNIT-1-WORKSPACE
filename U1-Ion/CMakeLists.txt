cmake_minimum_required(VERSION 3.15)

project(U1_ION VERSION 1.0.0)

# 1. Fetch JUCE
include(FetchContent)
FetchContent_Declare(
    juce
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG        8.0.4
)
FetchContent_MakeAvailable(juce)

# 2. Add Plugin (Standalone only)
juce_add_plugin(U1_ION
    COMPANY_NAME "NeuralBridge"
    PLUGIN_NAME "U1 Ion"
    PRODUCT_NAME "U1Ion"
    FORMATS Standalone AU VST3
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS TRUE
    COPY_STEP TRUE
    PLUGIN_CODE "U1IN"
    PLUGIN_MANUFACTURER_CODE "NBRI"
)

# 3. Source Files
target_sources(U1_ION PRIVATE
    Source/IonProcessor.cpp
    Source/IonProcessor.h
    Source/IonEditor.cpp
    Source/IonEditor.h
    Source/DSP/BaseDSPBlock.h
    Source/DSP/HorizonDrive.h
    Source/DSP/PVVitriol.h
    Source/DSP/CabSimulator.h
    Source/Utils/ModelLoader.h
)

# 4. Libraries
target_link_libraries(U1_ION PRIVATE
    juce::juce_audio_processors
    juce::juce_audio_utils
    juce::juce_dsp
    juce::juce_gui_extra
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags
)

# 5. Resources (Binary Data)
juce_add_binary_data(ION_RESOURCES
    SOURCES
    "ION_-_UI_v1_2026-Feb-12_11-49-41PM-000_CustomizedView15444265983.png"
    "Main_Backgroud_Power_Off.PNG"
    "Knob v2.png"
    "v003_01_Horizon_Pedal_Epochs1000_Chunk60s_Loss0p006719.json"
    "v005_02_Invective_Amp_Epochs300_Chunk2s_Loss0p002201.json"
)

target_link_libraries(U1_ION PRIVATE ION_RESOURCES)

target_compile_definitions(U1_ION PRIVATE
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
)
