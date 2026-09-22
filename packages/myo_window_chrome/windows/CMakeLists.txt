# The Flutter tooling requires that developers have a version of Visual Studio
# installed that includes CMake 3.14 or later. You should not increase this
# version, as doing so will cause the plugin to fail to compile for some
# customers of the plugin.
cmake_minimum_required(VERSION 3.14)

# Project-level configuration.
set(PROJECT_NAME "myo_window_chrome")
project(${PROJECT_NAME} LANGUAGES CXX)

cmake_policy(VERSION 3.14...3.25)

# This value is used when generating builds using this plugin, so it must
# not be changed.
set(PLUGIN_NAME "myo_window_chrome_plugin")

add_library(${PLUGIN_NAME} SHARED
  "include/myo_window_chrome/myo_window_chrome_plugin_c_api.h"
  "myo_window_chrome_plugin_c_api.cpp"
  "window_chrome.cpp"
)

apply_standard_settings(${PLUGIN_NAME})

# Symbols are hidden by default; window_chrome.h exports the one the runner
# calls.
set_target_properties(${PLUGIN_NAME} PROPERTIES
  CXX_VISIBILITY_PRESET hidden)
target_compile_definitions(${PLUGIN_NAME} PRIVATE FLUTTER_PLUGIN_IMPL)
target_compile_definitions(${PLUGIN_NAME} PRIVATE "NOMINMAX")

# PUBLIC, and not only for this library's own sources: the runner compiles the
# header above too, and these files carry prose in their comments. Without
# /utf-8 MSVC reads them in the machine's ANSI code page and raises C4819,
# which the runner's /WX turns into a build error. A ja-JP Windows host does
# exactly that.
target_compile_options(${PLUGIN_NAME} PUBLIC /utf-8)

# INTERFACE is what puts myo_window_chrome/window_chrome.h on the
# runner's include path once Flutter links this plugin into it.
target_include_directories(${PLUGIN_NAME} INTERFACE
  "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_include_directories(${PLUGIN_NAME} PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(${PLUGIN_NAME} PRIVATE flutter flutter_wrapper_plugin)
# The frame belongs to the desktop compositor; dwmapi is how it is asked to
# redraw one.
target_link_libraries(${PLUGIN_NAME} PRIVATE "dwmapi.lib")

# List of absolute paths to libraries that should be bundled with the plugin.
set(myo_window_chrome_bundled_libraries
  ""
  PARENT_SCOPE
)
