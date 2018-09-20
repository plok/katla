#include "gpu/opengl/opengl.h"

#include "window/window-factory.h"

#include "drawing/skia-opengl.h"

#include "timer-uv.h"
#include "event-loop-uv.h"

#include "string.h"
#include <iostream>
#include <vector>

// TODO remove
#include <GLFW/glfw3.h>

std::tuple<std::unique_ptr<GraphicsBackend>, ErrorPtr> initializeGraphicsBackend() {

    std::cout << "Creating opengl backend!" << std::endl;
    
    auto opengl = std::make_unique<OpenGl>();
    auto error = opengl->init();
    if (error) {
        return {std::unique_ptr<GraphicsBackend>(), Error::create("Failed initializing opengl backend: " + error->message)};
    }

    return {std::unique_ptr<GraphicsBackend>(std::move(opengl)), Error::none()};
}

int main(int argc, char* argv[])
{
    UvEventLoop eventLoop;
    eventLoop.init();
    
    auto [graphicsBackend, error] = initializeGraphicsBackend();
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

    UvTimer timer(&eventLoop);
    timer.init();
    
    SkiaOpenGL skia;
    skia.init();
    skia.draw();
    window->render();

    timer.start(17, [&skia, &window]() {
        skia.draw();
        window->render();
    });

    
    // window->waitForClose();

    eventLoop.run();

    graphicsBackend->cleanup();

    timer.stop();
    eventLoop.close();

    return 0;
}
