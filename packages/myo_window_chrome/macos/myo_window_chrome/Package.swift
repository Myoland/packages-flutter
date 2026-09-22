// swift-tools-version: 6.0
// The same sources the podspec compiles, as a Swift package: it is what makes
// the chrome testable with `swift test` on a Mac that has only the Command
// Line Tools, with no Xcode, no Flutter and no app build in the way.

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
        .target(name: "myo_window_chrome"),
        .testTarget(
            name: "myo_window_chromeTests",
            dependencies: ["myo_window_chrome"]
        ),
    ]
)
