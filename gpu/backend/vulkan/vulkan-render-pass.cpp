#include "vulkan-render-pass.h"

#include <utility>

#include "vulkan.h"
#include "vulkan-function-table.h"

VulkanRenderPass::VulkanRenderPass(
        std::shared_ptr<VulkanFunctionTable> vft,
        VulkanDevicePtr vulkanDevice,
        SwapChainResources swapChain,
        VkPipelineLayout pipeline) :
    m_functionTable(std::move(vft)),
    m_vulkanDevice(vulkanDevice),
    m_swapChain(swapChain),
    m_pipeline(pipeline),
    m_initialized(false)
{
}

VulkanRenderPass::~VulkanRenderPass()
{
    // TODO look at vulkan handle?
    if (m_initialized) {
        m_functionTable->DestroyRenderPass(m_vulkanDevice->vulkanHandle(), m_renderPass, nullptr);
    }
}

ErrorPtr VulkanRenderPass::init()
{
    VkAttachmentDescription colorAttachment = {};
    colorAttachment.format = m_swapChain.surfaceFormat.format;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef = {};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    VkSubpassDependency dependency = {};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    VkRenderPassCreateInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    if (m_functionTable->CreateRenderPass(m_vulkanDevice->vulkanHandle(), &renderPassInfo, nullptr, &m_renderPass) != VK_SUCCESS) {
        return Error::create("failed to create render pass!");
    }

    m_initialized = true;

    return Error::none();
}
