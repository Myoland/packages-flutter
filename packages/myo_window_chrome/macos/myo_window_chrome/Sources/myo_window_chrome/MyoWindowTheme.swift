import AppKit

/// The chrome a Myoland app's window wears on macOS.
///
/// The app draws its own canvas edge to edge, so the title bar is made
/// transparent and takes the window's background colour instead of sitting
/// above the content as a separate strip of system grey. Which also means the
/// title bar has to follow the app when the app changes appearance: nothing
/// else repaints it.
@MainActor
public enum MyoWindowTheme {
  /// SwiftWire's dark canvas: `0xFF16100C`, which is `SwColor.canvas` in
  /// `package:swiftwire_design`. It is what the chrome falls back to before
  /// the app has said what it is painting. An app with a different canvas
  /// passes its own.
  public static let defaultBackgroundColor = NSColor(
    srgbRed: 22.0 / 255.0,
    green: 16.0 / 255.0,
    blue: 12.0 / 255.0,
    alpha: 1
  )

  /// What the plugin applies before the app has said anything.
  public static func applyDefaults(to window: NSWindow) {
    apply(to: window, backgroundColor: defaultBackgroundColor, dark: true)
  }

  /// Anything left nil is left as it is, so a caller that knows only its
  /// canvas colour does not have to invent an appearance to go with it.
  public static func apply(
    to window: NSWindow,
    backgroundColor: NSColor? = nil,
    dark: Bool? = nil
  ) {
    if let dark {
      window.appearance = NSAppearance(named: dark ? .darkAqua : .aqua)
    }
    window.titlebarAppearsTransparent = true
    if let backgroundColor {
      window.backgroundColor = backgroundColor
    }
  }
}
