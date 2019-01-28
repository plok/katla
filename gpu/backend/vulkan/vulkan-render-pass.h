#ifndef VULKAN_RENDER_PASS_H
#define VULKAN_RENDER_PASS_H

#include "common/error.h"
#include "gpu/backend/vulkan/vulkan-device.h"
#include "gpu/backend/vulkan/vulkan-swapchain-resources.h"

#include <vulkan/vulkan_core.h>

#include <memory>
#include <vector>

class VulkanFunctionTable;

class VulkanRenderPass;
typedef std::shared_ptr<VulkanRenderPass> VulkanRenderPassPtr;

class VulkanRenderPass
{
public:
    VulkanRenderPass(std::shared_ptr<VulkanFunctionTable> vft, VulkanDevicePtr vulkanDevice, SwapChainResources swapChain, VkPipelineLayout pipeline);
    virtual ~VulkanRenderPass();
    
    ErrorPtr init();

    VkRenderPass vulkanHandle() {
        return m_renderPass;
    }

private:
    std::shared_ptr<VulkanFunctionTable> m_functionTable;
    VulkanDevicePtr m_vulkanDevice;

    SwapChainResources m_swapChain;
    VkPipelineLayout m_pipeline;
    VkRenderPass m_renderPass;

    bool m_initialized;
};

#endif