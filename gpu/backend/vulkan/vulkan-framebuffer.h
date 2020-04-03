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

class VulkanFrameBuffer;
typedef std::shared_ptr<VulkanFrameBuffer> VulkanFrameBufferPtr;

class VulkanFrameBuffer
{
public:
    VulkanFrameBuffer(
        VulkanFunctionTable& vk,
        VulkanDevice& vulkanDevice,
        VulkanRenderPassPtr renderPass,
        SwapChainResources swapChain,
        VkImageView swapChainImageView
        );
    virtual ~VulkanFrameBuffer();
    
    ErrorPtr init();

    VkFramebuffer handle() {
        return _framebuffer;
    }

private:
    VulkanFunctionTable& _vk;
    VulkanDevice& _device;

    VulkanRenderPassPtr _renderPass;
    SwapChainResources _swapChain;
    VkImageView _swapChainImageView;
    
    VkFramebuffer _framebuffer;


    bool _initialized;
};

#endif