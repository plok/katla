#include "katla/gtk4/c-api/gtk-application.h"
#include "katla/gtk4/gtk-application.h"

void register_gtk_platform() {
    katla::GtkApplication::registerPlatform();
}