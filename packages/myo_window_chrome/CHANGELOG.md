## 0.2.0

* The plugin now applies the chrome itself, as it registers. No app has to
  call anything from its `MainFlutterWindow`, `my_application.cc` or
  `win32_window.cpp`, and on Windows the theme-change message is handled
  through the registrar's window procedure delegate.
* Added `MyoWindowChrome.apply()`, for changing that choice from Dart.
* `ApplyWindowChrome` on Linux is idempotent and takes a null title, so a
  runner that wants the header bar in place before the window is realized can
  still call it.

## 0.1.0

* First release. The macOS, Linux and Windows title bar treatments lifted out
  of the SwiftWire apps, whose five desktop shells each carried their own copy.
  The macOS background colour is now a parameter, defaulting to the canvas
  those apps use.
