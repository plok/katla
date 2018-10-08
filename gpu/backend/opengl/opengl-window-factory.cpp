#include "opengl-window-factory.h"
#include "opengl-window.h"

#include <GLFW/glfw3.h>

OpenGlWindowFactory::OpenGlWindowFactory() = default;

std::tuple<WindowPtr, ErrorPtr> OpenGlWindowFactory::create(std::shared_ptr<RenderView> renderView, std::shared_ptr<WindowProperties> properties)
{
    GLFWwindow* window = glfwCreateWindow(properties->size.width, properties->size.height, properties->title.c_str(), nullptr, nullptr);
    if (!window) {
        return {std::shared_ptr<Window>(), Error::create("Error creating window")};
    }

    glfwMakeContextCurrent(window);

    // TODO load extensions?

    auto openglWindow = std::make_shared<OpenGlWindow>(window, renderView, properties);

    return {openglWindow, Error::none()};    
}