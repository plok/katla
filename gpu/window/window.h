#ifndef KATLA_WINDOW_H
#define KATLA_WINDOW_H

#include "common/error.h"
#include "common/subject.h"
#include "common/size.h"

#include <tuple>
#include <memory>
#include <string>

class Window;
typedef std::shared_ptr<Window> WindowPtr;

struct WindowProperties {
    Size size;
    std::string title;
    std::shared_ptr<Observer<Size>> sizeChanged;
    std::shared_ptr<Observer<bool>> closeRequested;
};

class Window {
public:
    virtual ~Window() = default;

    virtual void init() = 0;
    virtual void close() = 0;

    virtual void show() = 0;
    
    virtual void render() = 0;
    virtual void clear() = 0;

    virtual void processEvents() = 0;
    
};

#endif