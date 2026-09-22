#include "myo_window_chrome/window_chrome.h"

#include <gdk/gdk.h>
#ifdef GDK_WINDOWING_X11
#include <gdk/gdkx.h>
#endif

namespace myo {

void ApplyWindowChrome(GtkWindow* window, const gchar* title) {
  if (title == nullptr) {
    // A window that was never given a title answers NULL here, and GTK shows
    // the application name instead. Treat an empty one the same way, so a
    // second call cannot blank a title the first call set.
    title = gtk_window_get_title(window);
  }
  if (title != nullptr && *title == '\0') {
    title = nullptr;
  }

  // Being called twice is normal, and is the point: a runner may dress the
  // window before it is realized, and this package's plugin dresses it again
  // as it registers. Keep the header bar that is already there rather than
  // stacking a second one on it.
  GtkWidget* existing = gtk_window_get_titlebar(window);
  if (GTK_IS_HEADER_BAR(existing)) {
    if (title != nullptr) {
      gtk_header_bar_set_title(GTK_HEADER_BAR(existing), title);
    }
    return;
  }

  // A header bar can only be installed before the window is realized. GTK
  // warns on a realized window and unrealizes it to swap the decoration, and
  // an app that is already running takes its rendering surface down with it:
  // measured on GNOME 50 / GTK 3.24, a Flutter window treated this late comes
  // up black with "Failed to create platform view rendering surface". So the
  // plugin, which registers after the runner has realized the view, can only
  // set the title here -- the runner is the one that can dress the window.
  if (gtk_widget_get_realized(GTK_WIDGET(window))) {
    if (title != nullptr) {
      gtk_window_set_title(window, title);
    }
    return;
  }

  // Use a header bar when running in GNOME as this is the common style used
  // by applications and is the setup most users will be using (e.g. Ubuntu
  // desktop).
  // If running on X and not using GNOME then just use a traditional title bar
  // in case the window manager does more exotic layout, e.g. tiling.
  // If running on Wayland assume the header bar will work (may need changing
  // if future cases occur).
  gboolean use_header_bar = TRUE;
#ifdef GDK_WINDOWING_X11
  GdkScreen* screen = gtk_window_get_screen(window);
  if (GDK_IS_X11_SCREEN(screen)) {
    const gchar* wm_name = gdk_x11_screen_get_window_manager_name(screen);
    if (g_strcmp0(wm_name, "GNOME Shell") != 0) {
      use_header_bar = FALSE;
    }
  }
#endif
  if (use_header_bar) {
    GtkHeaderBar* header_bar = GTK_HEADER_BAR(gtk_header_bar_new());
    gtk_widget_show(GTK_WIDGET(header_bar));
    if (title != nullptr) {
      gtk_header_bar_set_title(header_bar, title);
    }
    gtk_header_bar_set_show_close_button(header_bar, TRUE);
    gtk_window_set_titlebar(window, GTK_WIDGET(header_bar));
  } else if (title != nullptr) {
    gtk_window_set_title(window, title);
  }
}

void ApplyWindowBrightness(gboolean dark) {
  GtkSettings* settings = gtk_settings_get_default();
  if (settings == nullptr) {
    return;
  }
  g_object_set(settings, "gtk-application-prefer-dark-theme", dark, nullptr);
}

}  // namespace myo
