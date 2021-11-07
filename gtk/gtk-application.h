#ifndef KATLA_GTK_APPLICATION_H
#define KATLA_GTK_APPLICATION_H

// #include "katla-window-management.h"

#include "katla/core/error.h"
#include "katla/core/subject.h"
#include "katla/core/size.h"

#include "katla/gtk/gtk-window-factory.h"
#include "katla/gtk/gtk-window.h"

#include <tuple>
#include <memory>
#include <string>
#include <vector>

// TODO to private
#include <gtk/gtk.h>

namespace katla {

// class Window;
// typedef std::shared_ptr<Window> WindowPtr;

// struct WindowProperties {
//     Size_32s size;
//     std::string title;

//     // TODO remove
//     std::shared_ptr<Observer<Size_32s>> sizeChanged;
//     std::shared_ptr<Observer<bool>> closeRequested;
// };

class GtkApplication {
public:
    GtkApplication();
    virtual ~GtkApplication();

    virtual void init(int argc, char* argv[], std::string appName);

    virtual std::shared_ptr<GtkWindowImpl> createWindow();

    // TODO integrate in event-loop or seperate process?
    virtual int run();

    std::unique_ptr<Subscription> onStartup(std::function<void(void)> startupCallback) {
        return m_onStartupSubject.subscribe(std::make_shared<katla::FuncObserver<void>>(startupCallback));
    };

    std::unique_ptr<Subscription> onActivate(std::function<void(void)> activateCallback) {
        return m_onActivateSubject.subscribe(std::make_shared<katla::FuncObserver<void>>(activateCallback));
    };

private:
    static void printHello();
    static void handleStartup(::GtkApplication* app, GtkApplication* self);
    static void handleActivate(::GtkApplication* app, GtkApplication* self);

    katla::Subject<void> m_onStartupSubject;
    katla::Subject<void> m_onActivateSubject;

    ::GtkApplication* _app {};

    std::unique_ptr<GtkWindowFactory> _windowFactory;

    std::vector<std::shared_ptr<GtkWindowImpl>> _windows;

    // TODO move to core-application
    int _argc;
    char** _argv;

    bool _isRunning;
};

}

#endif