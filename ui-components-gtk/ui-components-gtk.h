#ifndef KATLA_UI_COMPONENTS_GTK_H
#define KATLA_UI_COMPONENTS_GTK_H

// #include "katla-window-management.h"

#include "common/error.h"
#include "common/subject.h"
#include "common/size.h"

#include <gtk/gtk.h>

#include <tuple>
#include <memory>
#include <string>

// class Window;
// typedef std::shared_ptr<Window> WindowPtr;

// struct WindowProperties {
//     Size_32s size;
//     std::string title;

//     // TODO remove
//     std::shared_ptr<Observer<Size_32s>> sizeChanged;
//     std::shared_ptr<Observer<bool>> closeRequested;
// };

class UiComponentsGtk {
public:
    UiComponentsGtk();
    virtual ~UiComponentsGtk();

    virtual void init(int argc, char* argv[]);

    virtual void createWindow();

    // TODO integrate in event-loop or seperate process?
    virtual int run();

private:
    static void printHello();
    static void handleStartup(GtkApplication* app, UiComponentsGtk* self);
    static void handleActivate(GtkApplication* app, UiComponentsGtk* self);

    GtkApplication* _app;

    std::vector<GtkWidget*> _windows;

    // TODO move to core-application
    int _argc;
    char** _argv;

    bool _isRunning;
};

#endif
