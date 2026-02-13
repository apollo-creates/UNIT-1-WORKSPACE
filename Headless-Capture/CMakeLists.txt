cmake_minimum_required(VERSION 3.22)

project(HEADLESS_CAPTURE VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Fetch JUCE
include(FetchContent)
FetchContent_Declare(
    juce
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG        8.0.4 # Match Modeller project
)
FetchContent_MakeAvailable(juce)

juce_add_gui_app(HeadlessCapture
    PRODUCT_NAME "Headless Capture"
    COMPANY_NAME "UNIT 1"
    BUNDLE_ID "com.unit1.headlesscapture"
    IS_CONTENT_PREVIEW TRUE
)

# Source files
target_sources(HeadlessCapture PRIVATE
    Source/Main.cpp
    Source/MainComponent.cpp
    Source/MainComponent.h
    Source/PluginHostManager.cpp
    Source/PluginHostManager.h
    Source/CaptureController.cpp
    Source/CaptureController.h
)

# Link JUCE modules
target_link_libraries(HeadlessCapture PRIVATE
    juce::juce_audio_basics
    juce::juce_audio_devices
    juce::juce_audio_formats
    juce::juce_audio_processors
    juce::juce_audio_utils
    juce::juce_core
    juce::juce_data_structures
    juce::juce_events
    juce::juce_graphics
    juce::juce_gui_basics
    juce::juce_gui_extra
)

# Enable VST3 and AU hosting
target_compile_definitions(HeadlessCapture PRIVATE
    JUCE_PLUGINHOST_VST3=1
    JUCE_PLUGINHOST_AU=1
    JUCE_STRICT_REFCOUNTEDPOINTER=1
)

juce_generate_juce_header(HeadlessCapture)
