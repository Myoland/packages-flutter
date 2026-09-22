import AppKit
import Testing

@testable import myo_window_chrome

@Suite("macOS window chrome")
struct MyoWindowThemeTests {
  @MainActor
  @Test("the title bar is transparent over the design system's canvas")
  func themedWindowChrome() {
    let window = NSWindow(
      contentRect: NSRect(x: 0, y: 0, width: 800, height: 600),
      styleMask: [.titled, .closable, .miniaturizable, .resizable],
      backing: .buffered,
      defer: false
    )

    MyoWindowTheme.apply(to: window)

    let canvas = NSColor(
      srgbRed: 22.0 / 255.0,
      green: 16.0 / 255.0,
      blue: 12.0 / 255.0,
      alpha: 1
    )
    #expect(window.appearance?.name == .darkAqua)
    #expect(window.titlebarAppearsTransparent)
    #expect(window.backgroundColor == canvas)
  }
}
