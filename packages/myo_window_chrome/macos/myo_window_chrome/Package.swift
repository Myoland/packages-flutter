// swift-tools-version: 6.0
// The treatment, as a Swift package, so `swift test` can exercise it on a Mac
// with only the Command Line Tools: no Xcode, no Flutter, no app build.
//
// Deliberately only MyoWindowTheme.swift. MyoWindowChromePlugin.swift imports
// FlutterMacOS, which exists inside a Flutter build and nowhere else; the
// podspec beside this file compiles both.

import PackageDescription

let package = Package(
    name: "myo_window_chrome",
    platforms: [
        .macOS("13.0")
    ],
    products: [
        .library(name: "myo-window-chrome", targets: ["myo_window_chrome"])
    ],
    targets: [
        .target(name: "myo_window_chrome", sources: ["MyoWindowTheme.swift"]),
        .testTarget(
            name: "myo_window_chromeTests",
            dependencies: ["myo_window_chrome"]
        ),
    ]
)
