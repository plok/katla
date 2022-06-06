#include "gtk-application.h"

#include "gtk-window.h"
#include "katla/gtk3/gtk-window.h"
#include "katla/gtk3/gtk-button-impl.h"
#include "katla/gtk3/gtk-column.h"
#include "katla/gtk3/gtk-text-impl.h"

#include <gtk/gtk.h>

namespace katla {

GtkApplication::GtkApplication() :
    _app(nullptr),
    _argc(0),
    _argv(nullptr),
    _isRunning(false)
{}

GtkApplication::~GtkApplication()
{
    _windows.clear();

    // TODO use GObject template that automatically unrefs in restructor
    if (_app) {
        g_object_unref (_app);
        _app = nullptr;
    }
}

void GtkApplication::init(int argc, char* argv[], std::string appName)
{
    _argc = argc;
    _argv = argv;

    _app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);

    _windowFactory = std::make_unique<GtkWindowFactory>(_app);

    g_signal_connect (_app, "startup", G_CALLBACK (handleStartup), this);
    g_signal_connect (_app, "activate", G_CALLBACK (handleActivate), this);
     
}

void GtkApplication::printHello()
{
    std::cout << "Hello World!" << std::endl;
}

void GtkApplication::handleStartup(::GtkApplication* app, GtkApplication *self)
{
    self->_isRunning = true;
    self->m_onStartupSubject.next();
}

void GtkApplication::handleActivate(::GtkApplication* app, GtkApplication *self)
{
    std::cout << "activate!" << std::endl;

    self->m_onActivateSubject.next();
}

// TODO here or standalone with some kind of register?
std::shared_ptr<GtkWindowImpl> GtkApplication::createWindow(std::string title, katla::Size_32s size)
{
    if (!_isRunning) {
        std::cerr << "Cannot create window when application has not initialized" << std::endl;
        return {}; // TODO return window
    }

    auto createResult = _windowFactory->create(title, size);
    if (!createResult) {
        // TODO error
        return {};
    }
    std::shared_ptr<GtkWindowImpl> window = std::move(createResult.value());

    _windows.push_back(window);
    return window;
}

int GtkApplication::run()
{
    int result = g_application_run (G_APPLICATION (_app), _argc, _argv);
    _isRunning = false;

    return result;
}

}