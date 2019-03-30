#ifndef VULKAN_WINDOW_H
#define VULKAN_WINDOW_H

#include "window-management/window.h"
#include "common/error.h"

#include "gpu/backend/vulkan/vulkan-device.h"
#include "gpu/backend/vulkan/vulkan-function-table.h"
#include "gpu/backend/vulkan/vulkan-graphics-pipeline.h"
#include "gpu/backend/vulkan/vulkan-swapchain-resources.h"
#include "gpu/backend/vulkan/vulkan-engine.h"

#include <vulkan/vulkan.h>

#include <memory>

typedef struct GLFWwindow GLFWwindow;

class VulkanWindow;
typedef std::shared_ptr<VulkanWindow> VulkanWindowPtr;

class VulkanWindow : public Window {
public:
    // Takes ownership of window
    VulkanWindow(
        VulkanFunctionTable& vk,
        VulkanDevice& device,
        GLFWwindow* window,
        VkSurfaceKHR surface,
        SwapChainResources swapChainResources,
        VulkanGraphicsPipelinePtr graphicsPipeline,
        VulkanEnginePtr vulkanEngine);
    virtual ~VulkanWindow();

    void init();
    void show();
    void render();
    void clear() {}
    void close() {}

    void processEvents() {}

    VkSurfaceKHR vulkanSurface() {
        return m_surface;
    }

    void waitForClose();

    void attachCloseSubject(std::shared_ptr<Subject<bool>> closeSubject) {
        m_closeSubject = closeSubject;
    }

private:
    VulkanFunctionTable& _vk;
    VulkanDevice& _device;

    GLFWwindow* m_window;
    VkSurfaceKHR m_surface;
    SwapChainResources m_swapChainResources;
    VulkanGraphicsPipelinePtr m_graphicsPipeline;
    VulkanEnginePtr m_vulkanEngine;

    std::shared_ptr<Subject<bool>> m_closeSubject;
};

#endif
