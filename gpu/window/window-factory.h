#ifndef KATLA_WINDOW_FACTORY_H
#define KATLA_WINDOW_FACTORY_H

#include "window.h"

#include "common/error.h"

#include <tuple>

class RenderView;

class WindowFactory {
public:
    virtual ~WindowFactory() = default;

    virtual std::tuple<WindowPtr, ErrorPtr> create(std::shared_ptr<RenderView> renderView, std::shared_ptr<WindowProperties> properties) = 0;
};

#endif