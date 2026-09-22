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
// idiom, and a plain title bar everywhere else.
//
// Call this while the window is being built, before it is shown and before
// the FlView exists. GTK will not take a new title bar once the window is
// realized, and the plugin registrar that would otherwise be the natural
// place to do this does not exist yet.
MYO_WINDOW_CHROME_EXPORT void ApplyWindowChrome(GtkWindow* window,
                                                      const gchar* title);

}  // namespace myo

#endif  // MYO_WINDOW_CHROME_WINDOW_CHROME_H_
