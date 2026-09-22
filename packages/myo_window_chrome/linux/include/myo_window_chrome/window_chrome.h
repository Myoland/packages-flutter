#ifndef MYO_WINDOW_CHROME_WINDOW_CHROME_H_
#define MYO_WINDOW_CHROME_WINDOW_CHROME_H_

#include <gtk/gtk.h>

// A Flutter plugin library hides its symbols by default. This is the one
// function a runner calls, so it is the one symbol that leaves.
#ifdef FLUTTER_PLUGIN_IMPL
#define MYO_WINDOW_CHROME_EXPORT __attribute__((visibility("default")))
#else
#define MYO_WINDOW_CHROME_EXPORT
#endif

namespace myo {

// Gives |window| the title bar a Myoland app wears on this desktop: a
// header bar carrying |title| and a close button where that is the native
// idiom, and a plain title bar everywhere else. A null |title| keeps the
// window's current one.
//
// **The runner has to call this, before the window is realized.** GTK will
// not swap a title bar on a realized window without unrealizing it, which
// takes a running Flutter app's rendering surface down with it. The plugin
// calls this too, as it registers, but by then the window is realized and all
// it can do is set the title. Calling it twice is safe.
MYO_WINDOW_CHROME_EXPORT void ApplyWindowChrome(GtkWindow* window,
                                                      const gchar* title);

}  // namespace myo

#endif  // MYO_WINDOW_CHROME_WINDOW_CHROME_H_
