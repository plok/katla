#include "vulkan-framebuffer.h"

#include <utility>

#include "vulkan.h"
#include "vulkan-function-table.h"

#include <string>
#include <fstream>
#include <sstream>

VulkanFrameBuffer::VulkanFrameBuffer(
        VulkanFunctionTable& vk,
        VulkanDevice& vulkanDevice,
        VulkanRenderPassPtr renderPass, 
        SwapChainResources swapChain,
        VkImageView swapChainImageView) :
    _vk(vk),
    _device(vulkanDevice),
    _renderPass(renderPass),
    _swapChain(swapChain),
    _swapChainImageView(swapChainImageView),
    _initialized(false)
{
}

VulkanFrameBuffer::~VulkanFrameBuffer()
{
    // TODO look at vulkan handle?
    if (_initialized) {
        _vk.DestroyFramebuffer(_device.handle(), _framebuffer, nullptr);
    }
}

ErrorPtr VulkanFrameBuffer::init()
{
    VkImageView attachments[] = {
        _swapChainImageView
    };

    VkFramebufferCreateInfo framebufferInfo = {};
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.renderPass = _renderPass->handle();
    framebufferInfo.attachmentCount = 1;
    framebufferInfo.pAttachments = attachments;
    framebufferInfo.width = _swapChain.extent.width;
    framebufferInfo.height = _swapChain.extent.height;
    framebufferInfo.layers = 1;

    if (_vk.CreateFramebuffer(_device.handle(), &framebufferInfo, nullptr, &_framebuffer) != VK_SUCCESS) {
        throw Error::create("failed to create framebuffer!");
    }

    _initialized = true;

    return Error::none();
}
