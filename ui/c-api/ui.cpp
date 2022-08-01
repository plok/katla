#include "ui.h"

#include "katla/ui/application.h"
#include "katla/window-management/window.h"
#include "katla/ui/text.h"
#include "katla/ui/column.h"
#include "katla/ui/button.h"

void* create_application(int argc, char* argv[], char* appName) {
    auto app = katla::Application::create(argc, argv, appName);
    return app.release();
}
void destroy_application(void* app) {
    delete reinterpret_cast<katla::Application*>(app);
}

void* application_on_startup(void* app_, void (*startup_callback)(void* cb_app)) {
    auto app = reinterpret_cast<katla::Application*>(app_);
    auto sub = app->onStartup([startup_callback, app_]() { startup_callback(app_); });
    return sub.release();
}

void* application_on_activate(void* app_, void (*activate_callback)(void* cb_app)) {
    auto app = reinterpret_cast<katla::Application*>(app_);
    auto sub = app->onActivate([activate_callback, app_]() { activate_callback(app_); });
    return sub.release();
}

int application_run(void* app_) {
    auto app = reinterpret_cast<katla::Application*>(app_);
    return app->run();
}

void* application_create_window(void* app_, char* title, cSize_32s size) {
    auto app = reinterpret_cast<katla::Application*>(app_);
    return app->createWindow(title, {size.width, size.height}).get();
}

void destroy_subscription(void* sub_) {
    delete reinterpret_cast<katla::Subscription*>(sub_);
}

void window_show(void* window_) {
    auto window = reinterpret_cast<katla::Window*>(window_);
    window->show();
}

void window_set_child(void* window_, void* child) {
    auto window = reinterpret_cast<katla::Window*>(window_);
    auto widget = reinterpret_cast<katla::Widget*>(child);
    auto ptr = std::unique_ptr<katla::Widget>(widget);
    
    window->updateContainer({
        .children = {katla::ContainerChild{.child = std::move(ptr)}}
    });
}

void* text_create(char* text) {
    auto label = katla::Text::create();
    label->updateText({std::string(text)});
    return label.release();
}

void* column_create() {
    auto column = katla::Column::create();
    return column.release();
}

void column_append_child(void* column_, void* child_) {
    auto column = reinterpret_cast<katla::Column*>(column_);
    auto widget = reinterpret_cast<katla::Widget*>(child_);
    auto ptr = std::unique_ptr<katla::Widget>(widget);

    column->updateContainer({
        .children = {katla::ContainerChild{.child = std::move(ptr)}}
    });
}

void* button_create() {
    auto button = katla::Button::create();
    return button.release();
}

void* button_on_clicked(void* button_, void (*on_clicked_callback)(void* cb_button)) {
    auto button = reinterpret_cast<katla::Button*>(button_);
    auto sub = button->onClicked([on_clicked_callback, button_]() { on_clicked_callback(button_); });
    return sub.release();
}