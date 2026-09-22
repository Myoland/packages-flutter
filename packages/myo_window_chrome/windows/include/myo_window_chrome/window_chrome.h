#ifndef MYO_WINDOW_CHROME_WINDOWS_WINDOW_CHROME_H_
#define MYO_WINDOW_CHROME_WINDOWS_WINDOW_CHROME_H_

#include <windows.h>

#include <optional>

// A Flutter plugin library hides its symbols by default. This is the one
// function a runner calls, so it is the one symbol that leaves.
#ifdef FLUTTER_PLUGIN_IMPL
#define MYO_WINDOW_CHROME_EXPORT __declspec(dllexport)
#else
#define MYO_WINDOW_CHROME_EXPORT __declspec(dllimport)
#endif

namespace myo {

// Gives |window|'s frame the title bar a Myoland app wears on this desktop.
//
// With no |dark| the frame follows the system's app brightness, which is what
// a Windows app does when it has nothing to say. An app that can be told to
// be light or dark regardless of the desktop passes what it is: the frame is
// the one surface the app cannot repaint itself.
//
// Call it as the window is created, and again whenever Windows says the theme
// changed. The frame is drawn by the desktop compositor, which does not
// re-ask.
MYO_WINDOW_CHROME_EXPORT void ApplyWindowChrome(
    HWND window, std::optional<bool> dark = std::nullopt);

}  // namespace myo

#endif  // MYO_WINDOW_CHROME_WINDOWS_WINDOW_CHROME_H_
