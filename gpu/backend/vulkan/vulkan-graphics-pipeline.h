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

class GraphicsPipeline;
typedef std::shared_ptr<GraphicsPipeline> GraphicsPipelinePtr;

class GraphicsPipeline
{
public:
    GraphicsPipeline(std::shared_ptr<VulkanFunctionTable> vft, VulkanDevicePtr vulkanDevice, SwapChainResources swapChain);
    virtual ~GraphicsPipeline();
    
    ErrorPtr init();

    VkPipeline vulkanHandle() {
        return m_graphicsPipeline;
    }

    VulkanRenderPassPtr renderPass() {
        return m_renderPass;
    }

private:
    std::shared_ptr<VulkanFunctionTable> m_functionTable;
    VulkanDevicePtr m_vulkanDevice;

    SwapChainResources m_swapChain;
    VkPipelineLayout m_pipelineLayout;

    VulkanRenderPassPtr m_renderPass;

    VkPipeline m_graphicsPipeline;

    VulkanShaderPtr m_vertShader;
    VulkanShaderPtr m_fragShader;

    bool m_initialized;
};

#endif