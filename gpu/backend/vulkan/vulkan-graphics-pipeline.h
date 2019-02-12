#ifndef VULKAN_GRAPHICS_PIPELINE_H
#define VULKAN_GRAPHICS_PIPELINE_H

#include "common/error.h"
#include "gpu/backend/vulkan/vulkan-device.h"
#include "gpu/backend/vulkan/vulkan-swapchain-resources.h"
#include "gpu/backend/vulkan/vulkan-render-pass.h"
#include "gpu/backend/vulkan/vulkan-shader.h"

#include <vulkan/vulkan_core.h>

#include <memory>
#include <vector>

class VulkanFunctionTable;

class VulkanGraphicsPipeline;
typedef std::shared_ptr<VulkanGraphicsPipeline> VulkanGraphicsPipelinePtr;

class VulkanGraphicsPipeline
{
public:
    VulkanGraphicsPipeline(
        VulkanFunctionTable& vk,
        VulkanDevice& vulkanDevice,
        SwapChainResources& swapChain);
    virtual ~VulkanGraphicsPipeline();
    
    ErrorPtr init();

    VkPipeline handle() {
        return _graphicsPipeline;
    }

    VulkanRenderPassPtr renderPass() {
        return _renderPass;
    }

private:
    VulkanFunctionTable& _vk;
    VulkanDevice& _device;

    SwapChainResources& _swapChain;
    VkPipelineLayout _pipelineLayout;

    VulkanRenderPassPtr _renderPass;

    VkPipeline _graphicsPipeline;

    VulkanShaderPtr _vertShader;
    VulkanShaderPtr _fragShader;

    bool _initialized;
};

#endif