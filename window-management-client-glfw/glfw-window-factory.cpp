#include "glfw-window-factory.h"
#include "glfw-window.h"

#include <GLFW/glfw3.h>

GlfwWindowFactory::GlfwWindowFactory(const GraphicsConfiguration& configuration) :
    _configuration(configuration)
{}

std::tuple<WindowPtr, ErrorPtr> GlfwWindowFactory::create(std::shared_ptr<RenderView> renderView, std::shared_ptr<WindowProperties> properties)
{
    GLFWwindow* window = glfwCreateWindow(properties->size.width, properties->size.height, properties->title.c_str(), nullptr, nullptr);
    if (!window) {
        return {std::shared_ptr<Window>(), Error::create("Error creating window")};
    }

    glfwMakeContextCurrent(window);

    // TODO setting?
    glfwSwapInterval(1); // Enable vsync

    // TODO load extensions?

    // if (_configuration.useImGui) {
    //     _binders.init(OpenGlRenderer::ImGuiGlfwOpengl, window);
    // }

    auto openglWindow = std::make_shared<GlfwWindow>(window, renderView, properties);

    return {openglWindow, Error::none()};    
}