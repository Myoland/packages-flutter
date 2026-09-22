#include "include/myo_window_chrome/myo_window_chrome_plugin.h"

// Deliberately empty.
//
// This package has nothing to register: it publishes ApplyWindowChrome, which
// the runner calls as it builds the window, long before an engine exists to
// hand out registrars. The plugin declaration is here for what it does to the
// build rather than to the runtime — `pluginClass` is what makes Flutter's
// generated_plugins.cmake link this library into the runner and put its
// include directory on the runner's path. The alternative declaration,
// `ffiPlugin`, compiles and bundles the library but never links it into the
// runner, which is the one thing that would not work.
//
// If this ever stops being empty, look first at whether the work really
// belongs to the window or to the engine.
void myo_window_chrome_plugin_register_with_registrar(FlPluginRegistrar*) {}
