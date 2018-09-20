#include "gpu/vulkan/vulkan.h"
#include "gpu/opengl/opengl.h"

#include "window/window-factory.h"

#include "drawing/skia-opengl.h"

#include "string.h"
#include <iostream>
#include <vector>

// TODO remove
#include <GLFW/glfw3.h>

std::tuple<std::unique_ptr<GraphicsBackend>, ErrorPtr> initializeGraphicsBackend(bool useOpenGL) {
    if (useOpenGL) {
        std::cout << "Creating opengl backend!" << std::endl;
        
        auto opengl = std::make_unique<OpenGl>();
        auto error = opengl->init();
        if (error) {
            return {std::unique_ptr<GraphicsBackend>(), Error::create("Failed initializing opengl backend: " + error->message)};
        }
    
        return {std::unique_ptr<GraphicsBackend>(std::move(opengl)), Error::none()};
    }

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
    bool useOpenGL = false;

    std::vector<std::string> args;
    for (int i=0; i<argc; i++) {
        std::string arg = argv[i];
        args.push_back(arg);
    }

    for(auto arg : args) {
        if (arg == "--opengl") {
            useOpenGL = true;
        }
    }
        
    auto [graphicsBackend, error] = initializeGraphicsBackend(useOpenGL);
    if (error) {
        std::cout << "Failed initializing graphics backend" << error->message;
        return -1;
    }

    // before we can create a sufrace we need to create a window
    auto windowFactory = graphicsBackend->windowFactory();
    auto [window, createError] = windowFactory->create(800, 600, "Hello!");
    if (createError) {
        std::cout << "Failed creating window" << createError->message;
        return -1;
    }

    auto initDeviceError = graphicsBackend->initDevice();

    window->show();

    SkiaOpenGL skia;
    skia.init();
    skia.draw();

    window->render();
    window->waitForClose();

    graphicsBackend->cleanup();

    return 0;
}
