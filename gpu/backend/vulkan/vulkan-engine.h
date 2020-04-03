#ifndef VULKAN_ENGINE_H
#define VULKAN_ENGINE_H

#include "common/error.h"

#include "gpu/backend/vulkan/vulkan-device.h"
#include "gpu/backend/vulkan/vulkan-render-pass.h"
#include "gpu/backend/vulkan/vulkan-framebuffer.h"
#include "gpu/backend/vulkan/vulkan-command-pool.h"
#include "gpu/backend/vulkan/vulkan-graphics-pipeline.h"
#include "gpu/backend/vulkan/vulkan-semaphore.h"

#include <vulkan/vulkan_core.h>

#include <memory>
#include <vector>
#include <cstdint>

class VulkanFunctionTable;

class VulkanEngine;
typedef std::shared_ptr<VulkanEngine> VulkanEnginePtr;

class VulkanEngine
{
public:
    VulkanEngine(
        VulkanFunctionTable& vk,
        VulkanDevice& vulkanDevice,
        SwapChainResources swapChain,
        VulkanGraphicsPipelinePtr graphicsPipeline);
    virtual ~VulkanEngine();
    
    ErrorPtr init();

    ErrorPtr render();

private:
    ErrorPtr initFrameBuffers();
    ErrorPtr initCommandBuffers();
    ErrorPtr initSemaphores();

    VulkanFunctionTable& _vk;
    VulkanDevice& _device;

    SwapChainResources _swapChain;
    VulkanGraphicsPipelinePtr _graphicsPipeline;

    VulkanCommandPoolPtr _commandPool;

    std::vector<VulkanFrameBufferPtr> _swapChainFrameBuffers;
    std::vector<VkCommandBuffer> _commandBuffers;

    VulkanSemaphorePtr _imageAvailableSemaphore;
    VulkanSemaphorePtr _renderFinishedSemaphore;

    bool _initialized;
};

#endif