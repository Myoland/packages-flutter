## 0.2.3

* Linux: never swap the title bar on a realized window. GTK warns and
  unrealizes the window to do it, which takes a running Flutter app's
  rendering surface with it -- the window comes up black. Measured on GNOME 50
  with GTK 3.24.52; an earlier check under Xvfb missed it because nothing was
  really being composited. The plugin now only refreshes the title on a
  realized window, so a Linux app still calls `myo::ApplyWindowChrome` from its
  runner to get a header bar.

## 0.2.2

* The podspec reads its version from `pubspec.yaml` instead of keeping a second
  copy, which had already drifted: 0.2.1 installed as pod `0.1.0`.

## 0.2.1

* Import FlutterMacOS `@preconcurrency`. `FlutterMethodNotImplemented` is a
  global `var` in the engine's ObjC headers, which Swift 6 rejects as shared
  mutable state, so 0.2.0 did not compile in an app built with Swift 6.

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
