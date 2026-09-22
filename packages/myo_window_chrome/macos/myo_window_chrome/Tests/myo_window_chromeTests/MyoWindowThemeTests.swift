import AppKit
import Testing

@testable import myo_window_chrome

@Suite("macOS window chrome")
struct MyoWindowThemeTests {
  @MainActor
  private func makeWindow() -> NSWindow {
    NSWindow(
      contentRect: NSRect(x: 0, y: 0, width: 800, height: 600),
      styleMask: [.titled, .closable, .miniaturizable, .resizable],
      backing: .buffered,
      defer: false
    )
  }

  @MainActor
  @Test("the title bar is transparent over the design system's canvas")
  func defaults() {
    let window = makeWindow()

    MyoWindowTheme.applyDefaults(to: window)

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

  @MainActor
  @Test("a light app gets a light title bar over its own canvas")
  func followsTheAppToLight() {
    let window = makeWindow()
    MyoWindowTheme.applyDefaults(to: window)

    let paper = NSColor(srgbRed: 250.0 / 255.0, green: 247.0 / 255.0,
                        blue: 243.0 / 255.0, alpha: 1)
    MyoWindowTheme.apply(to: window, backgroundColor: paper, dark: false)

    #expect(window.appearance?.name == .aqua)
    #expect(window.titlebarAppearsTransparent)
    #expect(window.backgroundColor == paper)
  }

  @MainActor
  @Test("what a caller leaves out is left alone")
  func partialUpdatesKeepTheRest() {
    let window = makeWindow()
    MyoWindowTheme.applyDefaults(to: window)

    MyoWindowTheme.apply(to: window, backgroundColor: nil, dark: false)
    #expect(window.appearance?.name == .aqua)
    #expect(window.backgroundColor == MyoWindowTheme.defaultBackgroundColor)

    let paper = NSColor(srgbRed: 1, green: 1, blue: 1, alpha: 1)
    MyoWindowTheme.apply(to: window, backgroundColor: paper, dark: nil)
    #expect(window.appearance?.name == .aqua)
    #expect(window.backgroundColor == paper)
  }
}
