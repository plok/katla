#ifndef KATLA_WINDOW_H
#define KATLA_WINDOW_H

#include "common/error.h"

#include <tuple>
#include <memory>

class Window;
typedef std::shared_ptr<Window> WindowPtr;

class Window {
public:
    virtual ~Window() = default;

    virtual void show() = 0;
    virtual void render() = 0;
    virtual void clear() = 0;

    virtual void waitForClose() = 0;
};

#endif