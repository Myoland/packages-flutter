#include "myo_window_chrome_plugin.h"

#include <optional>
#include <utility>
#include <variant>

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
      [this](HWND hwnd, UINT message, WPARAM, LPARAM) -> std::optional<LRESULT> {
        if (message == WM_DWMCOLORIZATIONCOLORCHANGED) {
          // Re-assert what the app last asked for. Without it a desktop theme
          // change would drag a deliberately light or dark app along with it.
          ApplyWindowChrome(hwnd, last_dark_);
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
  // by the compositor, which takes a brightness and nothing else.
  std::optional<bool> dark;
  if (const auto* arguments =
          std::get_if<flutter::EncodableMap>(method_call.arguments())) {
    const auto entry = arguments->find(flutter::EncodableValue("dark"));
    if (entry != arguments->end()) {
      if (const auto* value = std::get_if<bool>(&entry->second)) {
        dark = *value;
      }
    }
  }
  last_dark_ = dark;

  HWND window = Window();
  if (window != nullptr) {
    ApplyWindowChrome(window, dark);
  }
  result->Success();
}

}  // namespace myo
