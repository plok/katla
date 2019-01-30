#include "vulkan-framebuffer.h"

#include <utility>

#include "vulkan.h"
#include "vulkan-function-table.h"

#include <string>
#include <fstream>
#include <sstream>

VulkanFrameBuffer::VulkanFrameBuffer(
        std::shared_ptr<VulkanFunctionTable> vft,
        VulkanDevicePtr vulkanDevice,
        VulkanRenderPassPtr renderPass, 
        SwapChainResources swapChain,
        VkImageView swapChainImageView) :
    m_functionTable(std::move(vft)),
    m_vulkanDevice(vulkanDevice),
    m_renderPass(renderPass),
    m_swapChain(swapChain),
    m_swapChainImageView(swapChainImageView),
    m_initialized(false)
{
}

VulkanFrameBuffer::~VulkanFrameBuffer()
{
    // TODO look at vulkan handle?
    if (m_initialized) {
        m_functionTable->DestroyFramebuffer(m_vulkanDevice->vulkanHandle(), m_framebuffer, nullptr);
    }
}

ErrorPtr VulkanFrameBuffer::init()
{
    VkImageView attachments[] = {
        m_swapChainImageView
    };

    VkFramebufferCreateInfo framebufferInfo = {};
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.renderPass = m_renderPass->vulkanHandle();
    framebufferInfo.attachmentCount = 1;
    framebufferInfo.pAttachments = attachments;
    framebufferInfo.width = m_swapChain.extent.width;
    framebufferInfo.height = m_swapChain.extent.height;
    framebufferInfo.layers = 1;

    if (m_functionTable->CreateFramebuffer(m_vulkanDevice->vulkanHandle(), &framebufferInfo, nullptr, &m_framebuffer) != VK_SUCCESS) {
        throw Error::create("failed to create framebuffer!");
    }

    m_initialized = true;

    return Error::none();
}
