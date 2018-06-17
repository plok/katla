#ifndef KATLA_WINDOW_FACTORY_H
#define KATLA_WINDOW_FACTORY_H

#include "window.h"

#include "common/error.h"

#include <tuple>

class WindowFactory {
public:
    virtual ~WindowFactory() = default;

    virtual std::tuple<WindowPtr, ErrorPtr> create(int x, int y, std::string title) = 0;
};

#endif