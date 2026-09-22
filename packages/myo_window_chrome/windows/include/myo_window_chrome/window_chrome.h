#ifndef MYO_WINDOW_CHROME_WINDOWS_WINDOW_CHROME_H_
#define MYO_WINDOW_CHROME_WINDOWS_WINDOW_CHROME_H_

#include <windows.h>

// A Flutter plugin library hides its symbols by default. This is the one
// function a runner calls, so it is the one symbol that leaves.
#ifdef FLUTTER_PLUGIN_IMPL
#define MYO_WINDOW_CHROME_EXPORT __declspec(dllexport)
#else
#define MYO_WINDOW_CHROME_EXPORT __declspec(dllimport)
#endif

namespace myo {

// Gives |window|'s frame the title bar a Myoland app wears on this
// desktop, which on Windows means following the system's app brightness: dark
// decorations when the desktop is in dark mode, the default otherwise.
//
// Call it as the window is created, and again whenever Windows says the theme
// changed. The frame is drawn by the desktop compositor, which does not
// re-ask.
MYO_WINDOW_CHROME_EXPORT void ApplyWindowChrome(HWND window);

}  // namespace myo

#endif  // MYO_WINDOW_CHROME_WINDOWS_WINDOW_CHROME_H_
