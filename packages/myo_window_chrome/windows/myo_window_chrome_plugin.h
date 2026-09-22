#ifndef MYO_WINDOW_CHROME_PLUGIN_H_
#define MYO_WINDOW_CHROME_PLUGIN_H_

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>
#include <flutter/standard_method_codec.h>

#include <memory>

namespace myo {

class MyoWindowChromePlugin : public flutter::Plugin {
 public:
  static void RegisterWithRegistrar(flutter::PluginRegistrarWindows* registrar);

  explicit MyoWindowChromePlugin(flutter::PluginRegistrarWindows* registrar);
  ~MyoWindowChromePlugin() override;

  MyoWindowChromePlugin(const MyoWindowChromePlugin&) = delete;
  MyoWindowChromePlugin& operator=(const MyoWindowChromePlugin&) = delete;

  void HandleMethodCall(
      const flutter::MethodCall<flutter::EncodableValue>& method_call,
      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);

 private:
  HWND Window() const;

  flutter::PluginRegistrarWindows* registrar_;
  int window_proc_id_ = -1;
};

}  // namespace myo

#endif  // MYO_WINDOW_CHROME_PLUGIN_H_
