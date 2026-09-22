#include "include/myo_window_chrome/myo_window_chrome_plugin.h"

#include <cstring>

#include "include/myo_window_chrome/window_chrome.h"

namespace {

GtkWindow* WindowForRegistrar(FlPluginRegistrar* registrar) {
  FlView* view = fl_plugin_registrar_get_view(registrar);
  if (view == nullptr) {
    return nullptr;
  }
  GtkWidget* toplevel = gtk_widget_get_toplevel(GTK_WIDGET(view));
  return GTK_IS_WINDOW(toplevel) ? GTK_WINDOW(toplevel) : nullptr;
}

FlValue* Argument(FlMethodCall* method_call, const gchar* name,
                  FlValueType type) {
  FlValue* args = fl_method_call_get_args(method_call);
  if (args == nullptr || fl_value_get_type(args) != FL_VALUE_TYPE_MAP) {
    return nullptr;
  }
  FlValue* value = fl_value_lookup_string(args, name);
  if (value == nullptr || fl_value_get_type(value) != type) {
    return nullptr;
  }
  return value;
}

void MethodCallHandler(FlMethodChannel*, FlMethodCall* method_call,
                       gpointer user_data) {
  FlPluginRegistrar* registrar = FL_PLUGIN_REGISTRAR(user_data);
  g_autoptr(FlMethodResponse) response = nullptr;
  if (strcmp(fl_method_call_get_name(method_call), "apply") == 0) {
    // `background` is macOS's: the GTK title bar is drawn from the theme, not
    // painted by the app, so what it takes is a brightness.
    FlValue* dark = Argument(method_call, "dark", FL_VALUE_TYPE_BOOL);
    if (dark != nullptr) {
      myo::ApplyWindowBrightness(fl_value_get_bool(dark) ? TRUE : FALSE);
    }
    GtkWindow* window = WindowForRegistrar(registrar);
    if (window != nullptr) {
      FlValue* title = Argument(method_call, "title", FL_VALUE_TYPE_STRING);
      myo::ApplyWindowChrome(window,
                             title == nullptr ? nullptr
                                              : fl_value_get_string(title));
    }
    response = FL_METHOD_RESPONSE(fl_method_success_response_new(nullptr));
  } else {
    response = FL_METHOD_RESPONSE(fl_method_not_implemented_response_new());
  }

  g_autoptr(GError) error = nullptr;
  if (!fl_method_call_respond(method_call, response, &error)) {
    g_warning("Failed to answer a myo_window_chrome call: %s", error->message);
  }
}

}  // namespace

void myo_window_chrome_plugin_register_with_registrar(
    FlPluginRegistrar* registrar) {
  // Registration runs before the first frame, but not before the window is
  // realized -- the runner realizes the FlView, and that realizes the window.
  // So this refreshes a header bar the runner installed, or sets a title, and
  // deliberately does not try to install one. See window_chrome.cc.
  GtkWindow* window = WindowForRegistrar(registrar);
  if (window != nullptr) {
    myo::ApplyWindowChrome(window, nullptr);
  }

  g_autoptr(FlStandardMethodCodec) codec = fl_standard_method_codec_new();
  FlMethodChannel* channel = fl_method_channel_new(
      fl_plugin_registrar_get_messenger(registrar), "myo_window_chrome",
      FL_METHOD_CODEC(codec));
  fl_method_channel_set_method_call_handler(
      channel, MethodCallHandler, g_object_ref(registrar), g_object_unref);
  // The channel has to outlive this function. The registrar lives as long as
  // the engine, so hang it there rather than leaking it deliberately.
  g_object_set_data_full(G_OBJECT(registrar), "myo_window_chrome_channel",
                         channel, g_object_unref);
}
