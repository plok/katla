#include "vulkan-window.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

VulkanWindow::VulkanWindow(
        VulkanFunctionTable& vk,
        VulkanDevice& device,
        GLFWwindow* window,
        VkSurfaceKHR surface,
        SwapChainResources swapChainResources,
        VulkanGraphicsPipelinePtr graphicsPipeline,
        VulkanEnginePtr vulkanEngine) :
    _vk(vk),
    _device(device),
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
        _vk.DestroyImageView(_device.handle(), imageView, nullptr);
    }
    _vk.DestroySwapchainKHR(_device.handle(), m_swapChainResources.swapChain, nullptr);
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