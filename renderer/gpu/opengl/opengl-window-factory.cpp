#include "opengl-window-factory.h"
#include "opengl-window.h"

#include <GLFW/glfw3.h>

OpenGlWindowFactory::OpenGlWindowFactory()
{}

std::tuple<WindowPtr, ErrorPtr> OpenGlWindowFactory::create(int x, int y, std::string title, std::shared_ptr<WindowEvents> events)
{
    GLFWwindow* window = glfwCreateWindow(x, y, title.c_str(), NULL, NULL);
    if (!window) {
        return {std::shared_ptr<Window>(), Error::create("Error creating window")};
    }

    glfwMakeContextCurrent(window);

    // TODO load extensions?

    auto openglWindow = std::make_shared<OpenGlWindow>(window, events);

    return {openglWindow, Error::none()};    
}