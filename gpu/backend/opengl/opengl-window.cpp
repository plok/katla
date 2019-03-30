#include "opengl-window.h"

#include "graphics/render-view.h"

#include <GLFW/glfw3.h>

#include <iostream>
#include <utility>

OpenGlWindow::OpenGlWindow(
        GLFWwindow* window,
        std::shared_ptr<RenderView> renderView, 
        std::shared_ptr<WindowProperties> properties) :
    _window(window),
    _renderView(std::move(renderView)),
    _properties(std::move(properties)),
    _closeRequested(false)
{
}

OpenGlWindow::~OpenGlWindow() {
    if (_window) {
        glfwDestroyWindow(_window);
        _window = nullptr;
    }
}

void OpenGlWindow::init()
{
    if (!_window) {
        return;
    }

    glfwSetWindowUserPointer(_window, this);

    glfwSetWindowSizeCallback(_window, &windowSizeCallback);
    glfwSetWindowCloseCallback(_window, &windowCloseCallback);
    glfwSetFramebufferSizeCallback(_window, &framebufferSizeCallback);
}

void OpenGlWindow::show()
{
    int width = 0, height = 0;
    glfwGetFramebufferSize(_window, &width, &height);

    glViewport(0, 0, width, height);

    clear();
    render();
}

void OpenGlWindow::clear()
{
    if (!_window) {
        return;
    }

    glClearColor(1, 1, 1, 1);
    glClearStencil(0);
    glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void OpenGlWindow::close()
{
    if (!_window) {
        return;
    }

    _closeRequested = true;
    // TODO do not call from callback
    glfwDestroyWindow (_window);
}

void OpenGlWindow::render()
{

    if (!_window || _closeRequested) {
        return;
    }
    
    if (_renderView) {
        _renderView->render();
    }

    glfwSwapBuffers(_window);
    // glfwPollEvents();
}  

void OpenGlWindow::processEvents()
{
    glfwPollEvents();
}

void OpenGlWindow::windowSizeCallback(GLFWwindow *window, int width, int height)
{    
    auto userPtr = glfwGetWindowUserPointer(window);
    if (!userPtr) {
        return;
    }

    auto windowObj = static_cast<OpenGlWindow*>(userPtr);

    windowObj->windowSizeChanged(width, height);
}

void OpenGlWindow::windowSizeChanged(int width, int height)
{
    // std::cout << "size" << std::endl;

    // TODO schedule, not in callback
    if (_renderView) {
        _renderView->resize(width, height);
    }
    // TODO needed, not in callback?
    //render();

    // if (_events->sizeChanged) {

    //     Size size {width, height};
    //     _events->sizeChanged->publish(size);
    // }
}

void OpenGlWindow::windowFocusCallback(GLFWwindow * /*window*/)
{
    std::cout << "focus" << std::endl;
}

void OpenGlWindow::windowCloseCallback(GLFWwindow *window)
{
    auto userPtr = glfwGetWindowUserPointer(window);
    if (!userPtr) {
        return;
    }

    auto windowObj = static_cast<OpenGlWindow*>(userPtr);

    windowObj->windowCloseRequested();
}

void OpenGlWindow::windowCloseRequested()
{
    _closeRequested = false;

    if (_properties->closeRequested) {
        _properties->closeRequested->publish(true);
    }
}

void OpenGlWindow::framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    // std::cout << "fb size" << std::endl;

    auto userPtr = glfwGetWindowUserPointer(window);
    if (!userPtr) {
        return;
    }
    auto windowObj = static_cast<OpenGlWindow*>(userPtr);

    windowObj->framebufferSize(width, height);
}

void OpenGlWindow::framebufferSize(int width, int height)
{
    glViewport(0, 0, width, height);
    render();
}
