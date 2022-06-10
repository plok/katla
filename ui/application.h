#ifndef KATLA_APPLICATION_H
#define KATLA_APPLICATION_H

#include "katla/window-management/window.h"

// #include "katla/ui/window.h"

#include "katla/core/error.h"
#include "katla/core/subject.h"
#include "katla/core/size.h"

#include <memory>

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

class Application {
public:
    Application() = default;
    virtual ~Application() = default;

    static std::unique_ptr<Application> create(int argc, char* argv[], std::string appName);

    virtual void init(int argc, char* argv[], std::string appName) = 0;

    virtual std::shared_ptr<Window> createWindow(std::string title, katla::Size_32s size) = 0;

    virtual int run() = 0;

    virtual std::unique_ptr<Subscription> onStartup(std::function<void(void)> startupCallback) = 0;
    virtual std::unique_ptr<Subscription> onActivate(std::function<void(void)> activateCallback) = 0;
};

}

#endif
