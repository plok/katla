#include "gtk-window-factory.h"
#include "gtk-window.h"

#include <gtk/gtk.h>

namespace katla {
    katla::GtkWindowFactory::GtkWindowFactory(::GtkApplication *app) : _app(app) {}

    outcome::result<std::unique_ptr<GtkWindowImpl>, katla::Error>
    katla::GtkWindowFactory::create(std::string title, katla::Size_32s size) {
        ::GtkWidget *window = gtk_application_window_new(_app);

        gtk_window_set_title(GTK_WINDOW (window), title.c_str());
        gtk_window_set_default_size(GTK_WINDOW (window), size.width, size.height);

        return std::make_unique<GtkWindowImpl>(GTK_WINDOW(window));
    }
}

