#include "vulkan-window.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

VulkanWindow::VulkanWindow(
        std::shared_ptr<VulkanFunctionTable> functionTable,
        VulkanDevicePtr device,
        GLFWwindow* window,
        VkSurfaceKHR surface,
        VkSwapchainKHR swapChain) :
    m_functionTable(functionTable),
    m_device(device),
    m_window(window),
    m_surface(surface),
    m_swapChain(swapChain)
{}

VulkanWindow::~VulkanWindow() {
    if (m_window) {
        glfwDestroyWindow(m_window);
        m_window = nullptr;
    }

    // TODO: or when destroying the device?
    m_functionTable->DestroySwapchainKHR(m_device->vulkanHandle(), m_swapChain, nullptr);
}

void VulkanWindow::init()
{
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