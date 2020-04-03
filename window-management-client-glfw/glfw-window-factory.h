#ifndef GLFW_WINDOW_FACTORY_H
#define GLFW_WINDOW_FACTORY_H

#include "glfw-window.h"
#include "graphics/render-view.h"

#include "graphics/graphics-configuration.h"

#include "window-management/window.h"
#include "window-management/window-factory.h"
#include "common/error.h"

#include <memory>
#include <string>

class RenderView;

class GlfwWindowFactory : public WindowFactory {
public:
    GlfwWindowFactory(const GraphicsConfiguration& configuration);

    std::tuple<WindowPtr, ErrorPtr> create(std::shared_ptr<RenderView> renderView, std::shared_ptr<WindowProperties> properties);
private:
    GraphicsConfiguration _configuration;

};

#endif