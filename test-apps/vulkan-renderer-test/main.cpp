#include "gpu/backend/vulkan/vulkan.h"
#include "gpu/backend/opengl/opengl.h"

#include "window-management/window-factory.h"
#include "graphics/render-view.h"

#include "app-kit/core-application-uv.h"

#include "string.h"
#include <iostream>
#include <vector>

// TODO remove
#include <GLFW/glfw3.h>

std::tuple<std::shared_ptr<GraphicsBackend>, ErrorPtr> initializeGraphicsBackend() {

    std::cout << "Creating vulkan backend!" << std::endl;

    auto vulkan = std::make_unique<Vulkan>();
    auto error = vulkan->init();
    if (error) {
        return {std::unique_ptr<GraphicsBackend>(), Error::create("Failed initializing vulkan backend: " + error->message)};
    }

    return {std::unique_ptr<GraphicsBackend>(std::move(vulkan)), Error::none()};
}


int main(int argc, char* argv[])
{
    std::vector<std::string> args;
    for (int i=0; i<argc; i++) {
        std::string arg = argv[i];
        args.push_back(arg);
    }

    UvCoreApplication app;
    auto appError = app.init();
    if (appError) {
        std::cout << appError->name << " " << appError->message << std::endl << std::flush;
        return -1;
    }

    auto [graphicsBackend, error] = initializeGraphicsBackend();
    if (error) {
        std::cout << "Failed initializing graphics backend" << error->message;
        return -1;
    }

    auto initDeviceError = graphicsBackend->initDevice();
    if (initDeviceError) {
        std::cout << "Failed initializing graphics backend" << initDeviceError->message;
        return -1;
    }

    std::cout << "device initialized!" << std::endl;

    auto renderView = std::make_shared<RenderView>();
    auto windowProperties = std::make_shared<WindowProperties>();
    windowProperties->size = Size_32s {800, 600};

    // before we can create a sufrace we need to create a window
    auto windowFactory = graphicsBackend->windowFactory();
    auto [window, createError] = windowFactory->create(renderView, windowProperties);
    if (createError) {
        std::cout << "Failed creating window" << createError->message;
        return -1;
    }

    window->show();

    appError = app.run();
    if (appError) {
        std::cout << appError->name << " " << appError->message << std::endl;
        return -1;
    }

    graphicsBackend->cleanup();

    return 0;
}
