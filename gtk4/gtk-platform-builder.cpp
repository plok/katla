#include "gtk-platform-builder.h"

#include "katla/gtk4/gtk-application.h"
#include "katla/gtk4/gtk-column.h"
#include "katla/gtk4/gtk-button-impl.h"
#include "katla/gtk4/gtk-text-impl.h"

namespace katla {

std::unique_ptr<Application> GtkPlatformBuilder::createApplication(int argc, char* argv[], std::string appName)
{
    auto gtkApp = std::make_unique<GtkApplication>();
    gtkApp->init(argc, argv, appName); // todo result / exception...
    return gtkApp;
}

std::unique_ptr<Column> GtkPlatformBuilder::createColumn() {
    auto widget = std::make_unique<GtkColumn>();
    widget->init();
    return widget;
}
std::unique_ptr<Button> GtkPlatformBuilder::createButton() {
    auto widget = std::make_unique<GtkButtonImpl>();
    widget->init();
    return widget;
}
std::unique_ptr<Text> GtkPlatformBuilder::createText() {
    auto widget = std::make_unique<GtkTextImpl>();
    widget->init();
    return widget;
}

}