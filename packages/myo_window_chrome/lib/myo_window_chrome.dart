import 'dart:ui' show Color;

import 'package:flutter/services.dart';

/// The title bar an app's window wears, applied by the plugin itself.
///
/// The plugin applies its defaults as it registers, which happens before the
/// first frame is drawn, so a window looks right without Dart asking for
/// anything. Call [apply] only to change what it chose — passing the app's own
/// canvas colour, say, once the design system has resolved it.
abstract final class MyoWindowChrome {
  static const MethodChannel _channel = MethodChannel('myo_window_chrome');

  /// Re-applies the chrome to the app's window.
  ///
  /// [background] is the colour the transparent macOS title bar shows through
  /// to. It does nothing on Linux or Windows, where the title bar is drawn by
  /// the desktop rather than painted by the app.
  ///
  /// [title] is the text in the Linux header bar. macOS and Windows take their
  /// title from the window itself.
  ///
  /// Doing nothing is a valid outcome: on a platform this package does not
  /// treat, the call completes without error.
  static Future<void> apply({Color? background, String? title}) {
    return _channel.invokeMethod<void>('apply', <String, Object?>{
      if (background != null) 'background': background.toARGB32(),
      if (title != null) 'title': title,
    });
  }
}
