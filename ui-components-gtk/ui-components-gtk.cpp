#include "ui-components-gtk.h"

#include <iostream>

UiComponentsGtk::UiComponentsGtk() :
    _app(nullptr),
    _argc(0),
    _argv(nullptr),
    _isRunning(false)
{}

UiComponentsGtk::~UiComponentsGtk()
{
    for(auto window : _windows) {
        gtk_widget_destroy(window);
    }
    _windows.clear();

    // TODO use GObject template that automatically unrefs in restructor
    if (_app) {
        g_object_unref (_app);
        _app = nullptr;
    }
}

void UiComponentsGtk::init(int argc, char* argv[])
{
    
    _app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);

    // TODO create window on activate
    g_signal_connect (_app, "startup", G_CALLBACK (handleStartup), this);
    g_signal_connect (_app, "activate", G_CALLBACK (handleActivate), this);
     
}

void UiComponentsGtk::printHello()
{
    std::cout << "Hello World!" << std::endl;
}

void UiComponentsGtk::handleStartup(GtkApplication* app, UiComponentsGtk *self)
{
    self->_isRunning = true;
    std::cout << "startup!" << std::endl;
}

void UiComponentsGtk::handleActivate(GtkApplication* app, UiComponentsGtk *self)
{
    std::cout << "activate!" << std::endl;

    self->createWindow();
}

void UiComponentsGtk::createWindow()
{
    if (!_isRunning) {
        std::cerr << "Cannot create window when application has not initialized" << std::endl;
        return; // TODO return window
    }

    auto window = gtk_application_window_new (_app);
    gtk_window_set_title (GTK_WINDOW (window), "Hello!");
    gtk_window_set_default_size (GTK_WINDOW (window), 200, 200);

    GtkWidget *button;
    GtkWidget *button_box;

    button_box = gtk_button_box_new (GTK_ORIENTATION_HORIZONTAL);
    gtk_container_add (GTK_CONTAINER (window), button_box);

    button = gtk_button_new_with_label ("Hello World");
    g_signal_connect (button, "clicked", G_CALLBACK (printHello), NULL);
    gtk_container_add (GTK_CONTAINER (button_box), button);

    gtk_widget_show_all (window);

    _windows.push_back(window);
}

int UiComponentsGtk::run()
{
    int result = g_application_run (G_APPLICATION (_app), _argc, _argv);
    _isRunning = false;

    return result;
}

