import 'dart:ui' show Brightness, Color;

import 'package:flutter/services.dart';

/// The title bar an app's window wears, applied by the plugin itself.
///
/// The plugin applies its defaults as it registers, which happens before the
/// first frame is drawn, so a window looks right without Dart asking for
/// anything. Call [apply] to keep it in step with an app that can change its
/// own appearance — the chrome is the one surface Flutter does not repaint
/// when the theme does.
abstract final class MyoWindowChrome {
  static const MethodChannel _channel = MethodChannel('myo_window_chrome');

  /// Re-applies the chrome to the app's window. Anything left null is left as
  /// it is.
  ///
  /// [brightness] chooses between the light and dark title bar: the macOS
  /// appearance, and on Windows the frame's immersive dark mode, which
  /// otherwise follows the desktop. On Linux it asks GTK for the dark or light
  /// variant of the current theme, which is what a header bar is drawn from.
  ///
  /// [background] is the colour the transparent macOS title bar shows through
  /// to, and should be the canvas the app is painting — otherwise the chrome
  /// reads as a seam across the top of the window. It does nothing on Linux or
  /// Windows, where the title bar is drawn by the desktop rather than painted
  /// by the app.
  ///
  /// [title] is the text in the Linux header bar. macOS and Windows take their
  /// title from the window itself.
  ///
  /// Doing nothing is a valid outcome: on Android, on the web, or anywhere
  /// else this package does not treat, the call completes without error.
  static Future<void> apply({
    Color? background,
    Brightness? brightness,
    String? title,
  }) async {
    try {
      await _channel.invokeMethod<void>('apply', <String, Object?>{
        if (background != null) 'background': background.toARGB32(),
        if (brightness != null) 'dark': brightness == Brightness.dark,
        if (title != null) 'title': title,
      });
    } on MissingPluginException {
      // A platform with no window to dress.
    }
  }
}
