#include "opengl-window.h"

#include <GLFW/glfw3.h>

#include <iostream>

OpenGlWindow::OpenGlWindow(GLFWwindow* window) :
    m_window(window)
{}

OpenGlWindow::~OpenGlWindow() {
    if (m_window) {
        glfwDestroyWindow(m_window);
        m_window = nullptr;
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void OpenGlWindow::init()
{
    if (!m_window) {
        return;
    }

    glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
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

void OpenGlWindow::render()
{
    if (!m_window) {
        return;
    }
    
    glfwSwapBuffers(m_window);
}  

void OpenGlWindow::waitForClose()
{
    if (!m_window) {
        return;
    }

    while(!glfwWindowShouldClose(m_window)) {
        glfwPollEvents();
    }
}