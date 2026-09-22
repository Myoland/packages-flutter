import AppKit

/// The chrome a dark Myoland app's window wears on macOS.
///
/// The app draws its own canvas edge to edge, so the title bar is made
/// transparent and takes the window's background colour instead of sitting
/// above the content as a separate strip of system grey.
@MainActor
public enum MyoWindowTheme {
  /// SwiftWire's canvas: `0xFF16100C`, which is `SwColor.canvas` in
  /// `package:swiftwire_design`. The transparent title bar shows this colour,
  /// so a value that drifts from the design token is visible as a seam across
  /// the top of the window. An app with a different canvas passes its own.
  public static let defaultBackgroundColor = NSColor(
    srgbRed: 22.0 / 255.0,
    green: 16.0 / 255.0,
    blue: 12.0 / 255.0,
    alpha: 1
  )

  public static func apply(to window: NSWindow, backgroundColor: NSColor? = nil) {
    window.appearance = NSAppearance(named: .darkAqua)
    window.titlebarAppearsTransparent = true
    window.backgroundColor = backgroundColor ?? defaultBackgroundColor
  }
}
