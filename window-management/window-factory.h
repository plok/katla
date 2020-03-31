#ifndef KATLA_WINDOW_FACTORY_H
#define KATLA_WINDOW_FACTORY_H

#include "katla-window-management.h"

#include "window.h"

#include "common/error.h"

#include <tuple>

class RenderView;

class KATLA_WINDOW_MANAGEMENT_DECLSPEC WindowFactory {
public:
    WindowFactory();
    virtual ~WindowFactory() = default;

    virtual std::tuple<WindowPtr, ErrorPtr> create(std::shared_ptr<RenderView> renderView, std::shared_ptr<WindowProperties> properties) = 0;
};

#endif