#ifndef KATLA_WINDOW_FACTORY_H
#define KATLA_WINDOW_FACTORY_H

#include "window.h"

#include "common/error.h"

#include <tuple>

class WindowFactory {
public:
    virtual ~WindowFactory() = default;

    virtual std::tuple<WindowPtr, ErrorPtr> create(int x, int y, std::string title, std::shared_ptr<WindowEvents> events) = 0;
};

#endif