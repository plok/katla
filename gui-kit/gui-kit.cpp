#include "gui-kit.h"

#include "app-kit/core-application.h"

#include "gpu/backend/graphics-configuration.h"

#include "gpu/backend/opengl/opengl.h"
#include "gpu/backend/vulkan/vulkan.h"
#include "gpu/window/window-factory.h"

#include "gpu/render/render-view.h"

#include <iostream>
#include <utility>

GuiKit::GuiKit(CoreApplication& coreApplication) :
    _coreApp(&coreApplication)
{
}

GuiKit::~GuiKit() = default;

ErrorPtr GuiKit::init()
{
    GraphicsConfiguration configuration;
    configuration.graphicsBackend = GraphicsBackends::OpenGl;
    configuration.useSkia = true;
    configuration.useImGui = true;

    auto [graphicsBackend, backendError] = initializeGraphicsBackend(configuration);
    if (backendError) {
        std::cout << "Failed initializing graphics backend" << backendError->message;
        return backendError;
    }
    _graphicsBackend = std::move(graphicsBackend);

    auto initDeviceError = _graphicsBackend->initDevice();
    if (initDeviceError) {
        std::cout << "Failed creating window" << initDeviceError->message;
        return initDeviceError;
    }

    return Error::none();
}

ErrorPtr GuiKit::close()
{
    _graphicsBackend->cleanup();
    return Error::none();
}

std::tuple<std::shared_ptr<GraphicsBackend>, ErrorPtr> GuiKit::initializeGraphicsBackend(const GraphicsConfiguration& configuration) {
    if (configuration.graphicsBackend == GraphicsBackends::OpenGl) {
        std::cout << "Creating opengl backend!" << std::endl;
        
        auto opengl = std::make_unique<OpenGl>();
        auto error = opengl->init(configuration);
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

WindowPtr GuiKit::createWindow(std::shared_ptr<WindowProperties> windowProperties, std::shared_ptr<RenderView> renderView)
{
    // before we can create a surface we need to create a window
    auto windowFactory = _graphicsBackend->windowFactory();

    auto [window, createError] = windowFactory->create(renderView, std::move(windowProperties));
    if (createError) {
        std::cout << "Failed creating window" << createError->message;
        return std::shared_ptr<Window>();
    }

    // if (!windowEvents->closeRequested) {
    //     windowEvents->closeRequested = std::make_shared<Subject<bool>>();
    // }

    // windowEvents->closeRequested->subscribe(std::make_shared<FuncObserver<bool>>([&app, &timer, &window](bool) {
    //     std::cout << "close event!" << std::endl << std::flush;
    //     app.close();
    // });

    window->init();
    renderView->init();

    window->show();

    return window;
}


