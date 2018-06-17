#include "vulkan-window.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

VulkanWindow::VulkanWindow(GLFWwindow* window) :
    m_window(window)
{}

VulkanWindow::~VulkanWindow() {
    if (m_window) {
        glfwDestroyWindow(m_window);
        m_window = nullptr;
    }
}

void VulkanWindow::show()
{
}

void VulkanWindow::render()
{
}

void VulkanWindow::waitForClose()
{
    if (!m_window) {
        return;
    }

    while(!glfwWindowShouldClose(m_window)) {
        glfwPollEvents();
    }
}