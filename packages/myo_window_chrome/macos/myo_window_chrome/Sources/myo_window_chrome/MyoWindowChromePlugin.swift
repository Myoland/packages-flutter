import AppKit
// @preconcurrency: FlutterMethodNotImplemented is a global `var` in the
// engine's ObjC headers, which Swift 6 reads as shared mutable state.
@preconcurrency import FlutterMacOS

/// Applies ``MyoWindowTheme`` to the app's window, and lets Dart change what
/// it applied.
///
/// The work happens in `register(with:)`, not on a Dart call: registration
/// runs while the engine is starting and before the first frame, so the window
/// is never shown in its undressed state. A Dart call that arrives later only
/// overrides the choice.
@MainActor
public final class MyoWindowChromePlugin: NSObject, @preconcurrency FlutterPlugin {
  private let registrar: FlutterPluginRegistrar

  private init(registrar: FlutterPluginRegistrar) {
    self.registrar = registrar
  }

  // Flutter registers plugins synchronously on its main platform thread. Keep
  // the generated, nonisolated registrar API and assert that boundary.
  public nonisolated static func register(with registrar: FlutterPluginRegistrar) {
    nonisolated(unsafe) let platformRegistrar = registrar
    MainActor.assumeIsolated {
      let instance = MyoWindowChromePlugin(registrar: platformRegistrar)
      let channel = FlutterMethodChannel(
        name: "myo_window_chrome",
        binaryMessenger: platformRegistrar.messenger
      )
      platformRegistrar.addMethodCallDelegate(instance, channel: channel)
      instance.applyDefaults(allowingRetry: true)
    }
  }

  public func handle(_ call: FlutterMethodCall, result: @escaping FlutterResult) {
    guard call.method == "apply" else {
      result(FlutterMethodNotImplemented)
      return
    }
    let arguments = call.arguments as? [String: Any]
    var backgroundColor: NSColor?
    if let argb = arguments?["background"] as? NSNumber {
      backgroundColor = NSColor(argb: argb.uint32Value)
    }
    let dark = (arguments?["dark"] as? NSNumber)?.boolValue
    if let window = registrar.view?.window {
      MyoWindowTheme.apply(to: window, backgroundColor: backgroundColor, dark: dark)
    }
    result(nil)
  }

  /// - Parameter allowingRetry: at registration the view may not have been put
  ///   in a window yet. That is a one-turn-of-the-runloop problem, not a
  ///   permanent one, so the first attempt is allowed a second try.
  private func applyDefaults(allowingRetry: Bool) {
    guard let window = registrar.view?.window else {
      if allowingRetry {
        DispatchQueue.main.async { [weak self] in
          self?.applyDefaults(allowingRetry: false)
        }
      }
      return
    }
    MyoWindowTheme.applyDefaults(to: window)
  }
}

extension NSColor {
  /// Dart hands colours over as packed ARGB, which is what `Color.toARGB32()`
  /// produces.
  fileprivate convenience init(argb: UInt32) {
    self.init(
      srgbRed: CGFloat((argb >> 16) & 0xFF) / 255.0,
      green: CGFloat((argb >> 8) & 0xFF) / 255.0,
      blue: CGFloat(argb & 0xFF) / 255.0,
      alpha: CGFloat((argb >> 24) & 0xFF) / 255.0
    )
  }
}
