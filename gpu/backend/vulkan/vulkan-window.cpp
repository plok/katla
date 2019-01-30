#include "vulkan-window.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

VulkanWindow::VulkanWindow(
        std::shared_ptr<VulkanFunctionTable> functionTable,
        VulkanDevicePtr device,
        GLFWwindow* window,
        VkSurfaceKHR surface,
        SwapChainResources swapChainResources,
        GraphicsPipelinePtr graphicsPipeline,
        VulkanEnginePtr vulkanEngine) :
    m_functionTable(functionTable),
    m_device(device),
    m_window(window),
    m_surface(surface),
    m_swapChainResources(swapChainResources),
    m_graphicsPipeline(graphicsPipeline),
    m_vulkanEngine(vulkanEngine)
{}

VulkanWindow::~VulkanWindow() {
    if (m_window) {
        glfwDestroyWindow(m_window);
        m_window = nullptr;
    }


    // TODO: or when destroying the device?
    for (auto imageView : m_swapChainResources.swapChainImageViews) {
        m_functionTable->DestroyImageView(m_device->vulkanHandle(), imageView, nullptr);
    }
    m_functionTable->DestroySwapchainKHR(m_device->vulkanHandle(), m_swapChainResources.swapChain, nullptr);
}

void VulkanWindow::init()
{
}

void VulkanWindow::show()
{
    m_vulkanEngine->render();
}

void VulkanWindow::render()
{
    m_vulkanEngine->render();
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