#include "myo_window_chrome_plugin.h"

#include <optional>
#include <utility>

#include "include/myo_window_chrome/window_chrome.h"

namespace myo {

void MyoWindowChromePlugin::RegisterWithRegistrar(
    flutter::PluginRegistrarWindows* registrar) {
  auto channel =
      std::make_unique<flutter::MethodChannel<flutter::EncodableValue>>(
          registrar->messenger(), "myo_window_chrome",
          &flutter::StandardMethodCodec::GetInstance());

  auto plugin = std::make_unique<MyoWindowChromePlugin>(registrar);
  channel->SetMethodCallHandler(
      [plugin_pointer = plugin.get()](const auto& call, auto result) {
        plugin_pointer->HandleMethodCall(call, std::move(result));
      });
  // The channel outlives this function through the plugin the registrar owns.
  registrar->AddPlugin(std::move(plugin));
}

MyoWindowChromePlugin::MyoWindowChromePlugin(
    flutter::PluginRegistrarWindows* registrar)
    : registrar_(registrar) {
  // Registration runs while the engine is starting and before the first frame,
  // which is early enough that the window is never seen undressed.
  HWND window = Window();
  if (window != nullptr) {
    ApplyWindowChrome(window);
  }

  // The desktop compositor draws the frame and never re-asks, so the chrome
  // has to be put back whenever Windows says the theme changed. Doing it here
  // is what spares every app the edit in its own window procedure.
  window_proc_id_ = registrar_->RegisterTopLevelWindowProcDelegate(
      [](HWND hwnd, UINT message, WPARAM, LPARAM) -> std::optional<LRESULT> {
        if (message == WM_DWMCOLORIZATIONCOLORCHANGED) {
          ApplyWindowChrome(hwnd);
        }
        return std::nullopt;
      });
}

MyoWindowChromePlugin::~MyoWindowChromePlugin() {
  if (window_proc_id_ != -1) {
    registrar_->UnregisterTopLevelWindowProcDelegate(window_proc_id_);
  }
}

HWND MyoWindowChromePlugin::Window() const {
  flutter::FlutterView* view = registrar_->GetView();
  return view == nullptr ? nullptr : view->GetNativeWindow();
}

void MyoWindowChromePlugin::HandleMethodCall(
    const flutter::MethodCall<flutter::EncodableValue>& method_call,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
  if (method_call.method_name() != "apply") {
    result->NotImplemented();
    return;
  }
  // `background` is macOS's and `title` is Linux's: a Windows frame is drawn
  // by the compositor from the system's own colours.
  HWND window = Window();
  if (window != nullptr) {
    ApplyWindowChrome(window);
  }
  result->Success();
}

}  // namespace myo
