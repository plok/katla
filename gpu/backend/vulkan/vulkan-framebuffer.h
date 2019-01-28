#ifndef VULKAN_FRAMEBUFFER_H
#define VULKAN_FRAMEBUFFER_H

#include "common/error.h"

#include "gpu/backend/vulkan/vulkan-device.h"
#include "gpu/backend/vulkan/vulkan-render-pass.h"

#include <vulkan/vulkan_core.h>

#include <memory>
#include <vector>
#include <cstdint>

class VulkanFunctionTable;

class VulkanFramebuffer;
typedef std::shared_ptr<VulkanFramebuffer> VulkanFramebufferPtr;

class VulkanFramebuffer
{
public:
    VulkanFramebuffer(
        std::shared_ptr<VulkanFunctionTable> vft,
        VulkanDevicePtr vulkanDevice,
        VulkanRenderPassPtr renderPass,
        SwapChainResources swapChain,
        VkImageView swapChainImageView
        );
    virtual ~VulkanFramebuffer();
    
    ErrorPtr init();

    VkFramebuffer vulkanStructure() {
        return m_framebuffer;
    }

private:
    std::shared_ptr<VulkanFunctionTable> m_functionTable;
    VulkanDevicePtr m_vulkanDevice;

    VulkanRenderPassPtr m_renderPass;
    SwapChainResources m_swapChain;
    VkImageView m_swapChainImageView;
    
    VkFramebuffer m_framebuffer;


    bool m_initialized;
};

#endif