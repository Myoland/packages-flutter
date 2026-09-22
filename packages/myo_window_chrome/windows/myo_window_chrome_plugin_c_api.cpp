#include "include/myo_window_chrome/myo_window_chrome_plugin_c_api.h"

#include <flutter/plugin_registrar_windows.h>

#include "myo_window_chrome_plugin.h"

void MyoWindowChromePluginCApiRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar) {
  myo::MyoWindowChromePlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarManager::GetInstance()
          ->GetRegistrar<flutter::PluginRegistrarWindows>(registrar));
}
