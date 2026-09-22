#include "myo_window_chrome/window_chrome.h"

#include <dwmapi.h>

namespace myo {

namespace {

/// Window attribute that enables dark mode window decorations.
///
/// Redefined in case the developer's machine has a Windows SDK older than
/// version 10.0.22000.0.
/// See: https://docs.microsoft.com/windows/win32/api/dwmapi/ne-dwmapi-dwmwindowattribute
#ifndef DWMWA_USE_IMMERSIVE_DARK_MODE
#define DWMWA_USE_IMMERSIVE_DARK_MODE 20
#endif

/// Registry key for app theme preference.
///
/// A value of 0 indicates apps should use dark mode. A non-zero or missing
/// value indicates apps should use light mode.
constexpr const wchar_t kGetPreferredBrightnessRegKey[] =
    L"Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize";
constexpr const wchar_t kGetPreferredBrightnessRegValue[] =
    L"AppsUseLightTheme";

}  // namespace

void ApplyWindowChrome(HWND window) {
  DWORD light_mode;
  DWORD light_mode_size = sizeof(light_mode);
  // The wide entry point by name: this library is built on its own and does
  // not inherit the app's UNICODE definition.
  LSTATUS result = RegGetValueW(HKEY_CURRENT_USER,
                                kGetPreferredBrightnessRegKey,
                                kGetPreferredBrightnessRegValue,
                                RRF_RT_REG_DWORD, nullptr, &light_mode,
                                &light_mode_size);

  if (result == ERROR_SUCCESS) {
    BOOL enable_dark_mode = light_mode == 0;
    DwmSetWindowAttribute(window, DWMWA_USE_IMMERSIVE_DARK_MODE,
                          &enable_dark_mode, sizeof(enable_dark_mode));
  }
}

}  // namespace myo
