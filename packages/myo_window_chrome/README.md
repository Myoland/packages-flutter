# myo_window_chrome

The title bar a dark desktop app's window wears, in one place instead of copied
into every runner. It was lifted out of SwiftWire, whose five shells — the
macOS client, the macOS Admin app, the Linux client, the Linux Admin app and
the Windows client — each carried their own answer to "what does this window
look like before Flutter draws anything".

Window chrome is one of the few things a Flutter app cannot keep in Dart: it is
set on the native window while that window is being built, which is before the
engine starts and therefore before any Dart isolate or plugin registrar exists.
That is also why this package is shaped the way it is.

## What each platform gets

| Platform | Treatment |
| --- | --- |
| macOS | `darkAqua`, a transparent title bar, and the window painted in the app's canvas — `0xFF16100C` unless the caller passes its own — so the chrome reads as part of the app rather than a strip above it. |
| Linux | A GNOME header bar carrying the app's title and a close button; a plain title bar under any other X11 window manager, where a header bar fights tiling. |
| Windows | Dark frame decorations when the desktop is in dark mode, following `AppsUseLightTheme`. |

The treatments differ because the desktops do. macOS forces dark because the
app is dark; Windows follows the system. Unifying those two is a product
decision, not a packaging one, and this package deliberately does not make it.

## Using it

```yaml
dependencies:
  myo_window_chrome:
    git:
      url: https://github.com/Myoland/packages-flutter.git
      ref: myo_window_chrome-v0.1.0
      path: packages/myo_window_chrome
```

`flutter pub get` is the whole integration. Nothing is added to
`Runner.xcodeproj`, and no app CMake file has to mention this package: on macOS
CocoaPods links every pod into the Runner target, and on Linux and Windows
Flutter's generated `generated_plugins.cmake` links the plugin into the runner
and puts its include directory on the runner's path.

**macOS** — in `MainFlutterWindow.swift`:

```swift
import myo_window_chrome

MyoWindowTheme.apply(to: self)
```

**Linux** — in `my_application.cc`, as the window is built and before the
`FlView` is created:

```cpp
#include "myo_window_chrome/window_chrome.h"

myo::ApplyWindowChrome(window, "SwiftWire");
```

**Windows** — in `win32_window.cpp`, from `Create` and again on
`WM_DWMCOLORIZATIONCOLORCHANGED`:

```cpp
#include "myo_window_chrome/window_chrome.h"

myo::ApplyWindowChrome(window);
```

## The empty registrants

`myo_window_chrome_plugin_register_with_registrar` on Linux and
`MyoWindowChromePluginCApiRegisterWithRegistrar` on Windows do nothing,
on purpose. There is nothing to register — the runner calls
`ApplyWindowChrome` itself, before any registrar exists. They are there
because `pluginClass` is the declaration that makes Flutter link a native
library into the runner, and `ffiPlugin`, the declaration that generates no
registrant, produces a library the runner never links. macOS has no such
split: it is declared `ffiPlugin` and stays registrant-free.

## Tests

The macOS treatment has its own Swift Testing target, which needs neither
Xcode nor Flutter:

```sh
cd macos/myo_window_chrome
swift test
```

The Linux and Windows halves are covered by the apps that build them.
