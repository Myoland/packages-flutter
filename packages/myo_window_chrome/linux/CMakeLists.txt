# The Flutter tooling requires that developers have CMake 3.10 or later
# installed. You should not increase this version, as doing so will cause
# the plugin to fail to compile for some customers of the plugin.
cmake_minimum_required(VERSION 3.10)

# Project-level configuration.
set(PROJECT_NAME "myo_window_chrome")
project(${PROJECT_NAME} LANGUAGES CXX)

# This value is used when generating builds using this plugin, so it must
# not be changed.
set(PLUGIN_NAME "myo_window_chrome_plugin")

add_library(${PLUGIN_NAME} SHARED
  "myo_window_chrome_plugin.cc"
  "window_chrome.cc"
)

apply_standard_settings(${PLUGIN_NAME})

# Symbols are hidden by default; window_chrome.h exports the one the runner
# calls.
set_target_properties(${PLUGIN_NAME} PROPERTIES
  CXX_VISIBILITY_PRESET hidden)
target_compile_definitions(${PLUGIN_NAME} PRIVATE FLUTTER_PLUGIN_IMPL)

# INTERFACE is what puts myo_window_chrome/window_chrome.h on the
# runner's include path once Flutter links this plugin into it.
target_include_directories(${PLUGIN_NAME} INTERFACE
  "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_include_directories(${PLUGIN_NAME} PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(${PLUGIN_NAME} PRIVATE flutter)
target_link_libraries(${PLUGIN_NAME} PRIVATE PkgConfig::GTK)

# List of absolute paths to libraries that should be bundled with the plugin.
set(myo_window_chrome_bundled_libraries
  ""
  PARENT_SCOPE
)
