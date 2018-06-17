#include "opengl.h"

#include "window/window-factory.h"
#include "gpu/opengl/opengl-window-factory.h"

#include <GLFW/glfw3.h>

#include "string.h"
#include <iostream>
#include <vector>

#include <sstream>

void glfw_opengl_error_callback(int error, const char* description)
{
    std::cout << description << std::endl;
}

OpenGl::OpenGl()
{
}

OpenGl::~OpenGl()
{
    // TODO out of destructor?
    glfwTerminate();
}

ErrorPtr OpenGl::init()
{
    if (!glfwInit()) {
        return Error::create("Failed initializing GLFW!");
    }

    glfwSetErrorCallback(glfw_opengl_error_callback);

    return Error::none();
}

void OpenGl::cleanup()
{
    // TODO out of destructor?
    glfwTerminate();
}

std::unique_ptr<WindowFactory> OpenGl::windowFactory()
{
    return std::make_unique<OpenGlWindowFactory>();
}