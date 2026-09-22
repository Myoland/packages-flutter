# packages-flutter

Flutter and Dart packages shared across Myoland's apps. Each one lives under
`packages/` and is depended on by its path inside this repository:

```yaml
dependencies:
  <package>:
    git:
      url: https://github.com/Myoland/packages-flutter.git
      ref: <package>-v<version>
      path: packages/<package>
```

Tags are per package, `<package>-v<version>`, so one package can be released
without moving any other.

| Package | What it is |
| --- | --- |
| [myo_window_chrome](packages/myo_window_chrome) | The title bar a dark desktop app's window wears, for macOS, Linux and Windows. |
