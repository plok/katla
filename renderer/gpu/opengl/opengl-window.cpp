#include "opengl-window.h"

#include <GLFW/glfw3.h>

#include <iostream>

OpenGlWindow::OpenGlWindow(
        GLFWwindow* window,
        std::shared_ptr<WindowEvents> events) :
    m_window(window),
    m_events(events),
    m_closeRequested(false)
{
}

OpenGlWindow::~OpenGlWindow() {
    if (m_window) {
        glfwDestroyWindow(m_window);
        m_window = nullptr;
    }
}

void OpenGlWindow::init()
{
    if (!m_window) {
        return;
    }

    glfwSetWindowUserPointer(m_window, this);

    glfwSetWindowSizeCallback(m_window, &windowSizeCallback);
    glfwSetWindowCloseCallback(m_window, &windowCloseCallback);
    glfwSetFramebufferSizeCallback(m_window, &framebufferSizeCallback);
}

void OpenGlWindow::show()
{
    int width = 0, height = 0;
    glfwGetFramebufferSize(m_window, &width, &height);

    glViewport(0, 0, width, height);

    clear();
    render();
}

void OpenGlWindow::clear()
{
    if (!m_window) {
        return;
    }

    glClearColor(1, 1, 1, 1);
    glClearStencil(0);
    glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void OpenGlWindow::close()
{
    if (!m_window) {
        return;
    }

    m_closeRequested = true;
    // TODO do not call from callback
    glfwDestroyWindow (m_window);
}

void OpenGlWindow::render()
{

    if (!m_window || m_closeRequested) {
        return;
    }
    
    glfwSwapBuffers(m_window);
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
    if (m_events->sizeChanged) {

        Size size {width, height};
        m_events->sizeChanged->publish(size);
    }
}

void OpenGlWindow::windowFocusCallback(GLFWwindow *window)
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
    m_closeRequested = false;

    if (m_events->closeRequested) {
        m_events->closeRequested->publish(true);
    }
}

void OpenGlWindow::framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
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
}