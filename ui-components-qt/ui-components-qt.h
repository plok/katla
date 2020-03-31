#ifndef KATLA_UI_COMPONENTS_QT_H
#define KATLA_UI_COMPONENTS_QT_H

// #include "katla-window-management.h"

#include "common/error.h"
#include "common/subject.h"
#include "common/size.h"

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

class QApplication;
class QMainWindow;

class UiComponentsQt {
public:
    UiComponentsQt();
    virtual ~UiComponentsQt();

    virtual void init(int argc, char* argv[]);

    virtual void createWindow();

    // TODO integrate in event-loop or seperate process?
    virtual void run();

private:
    std::unique_ptr<QApplication> _app;

    std::vector<std::shared_ptr<QMainWindow>> _windows;
};

#endif
