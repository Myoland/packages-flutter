# myo_window_chrome

The title bar a dark desktop app's window wears, in one place instead of copied
into every runner. It was lifted out of SwiftWire, whose five shells — the
macOS client, the macOS Admin app, the Linux client, the Linux Admin app and
the Windows client — each carried their own answer to "what does this window
look like before Flutter draws anything".

## What each platform gets

| Platform | Treatment |
| --- | --- |
| macOS | `darkAqua`, a transparent title bar, and the window painted in the app's canvas — `0xFF16100C` unless the app passes its own — so the chrome reads as part of the app rather than a strip above it. |
| Linux | A GNOME header bar carrying the window's title and a close button; a plain title bar under any other X11 window manager, where a header bar fights tiling. |
| Windows | Dark frame decorations when the desktop is in dark mode, following `AppsUseLightTheme`, and put back whenever the desktop theme changes. |

The treatments differ because the desktops do. macOS forces dark because the
app is dark; Windows follows the system. Unifying those two is a product
decision, not a packaging one, and this package deliberately does not make it.

## Using it

```yaml
dependencies:
  myo_window_chrome:
    git:
      url: https://github.com/Myoland/packages-flutter.git
      ref: myo_window_chrome-v0.2.3
      path: packages/myo_window_chrome
```

`flutter pub get` is the whole integration on **macOS and Windows**: no native
file in the app is touched — not `MainFlutterWindow.swift`, not
`win32_window.cpp`, and no app CMake file or `Runner.xcodeproj`. The plugin
applies the chrome as it registers, which happens while the engine is starting
and before the first frame, so the window is never seen undressed and Dart does
not have to ask for anything.

**Linux needs one line in the runner**, for a reason worth knowing — see below.

To change what it chose — to hand it the colour your design system resolved,
for instance — call it:

```dart
await MyoWindowChrome.apply(background: SwColor.canvas);
```

`background` is macOS's: the Linux and Windows title bars are drawn by the
desktop, not painted by the app. `title` is Linux's: the other two take their
title from the window itself.

## Linux: the runner has to make the call

A GTK window is realized before any plugin registers — the runner realizes the
FlView, and that realizes the window — and GTK will not swap a title bar on a
realized window without unrealizing it first. Doing that to a running Flutter
app takes its rendering surface down: measured on GNOME 50 with GTK 3.24.52,
the window comes up black and the engine logs `Failed to create platform view
rendering surface`. So the plugin refuses to install a header bar that late; on
a realized window it only sets the title.

Which means a Linux app makes the call itself, from `my_application.cc`, while
the window is being built:

```cpp
#include "myo_window_chrome/window_chrome.h"

myo::ApplyWindowChrome(window, "My App");
```

The plugin still calls it afterwards, and calling twice is safe: the second
call keeps the header bar already there and only refreshes its title.

## Tests

The macOS treatment has its own Swift Testing target, which needs neither
Xcode nor Flutter:

```sh
cd macos/myo_window_chrome
swift test
```

The Linux and Windows halves are covered by the apps that build them.
