#include "vulkan-engine.h"

#include <utility>

#include "vulkan.h"
#include "vulkan-function-table.h"

#include <string>

VulkanEngine::VulkanEngine(
        std::shared_ptr<VulkanFunctionTable> vft,
        VulkanDevicePtr vulkanDevice,
        SwapChainResources swapChain,
        GraphicsPipelinePtr graphicsPipeline) :
    _functionTable(std::move(vft)),
    _device(vulkanDevice),
    _swapChain(swapChain),
    _graphicsPipeline(graphicsPipeline),
    _initialized(false)
{
}

VulkanEngine::~VulkanEngine()
{
    // TODO look at vulkan handle?
    if (_initialized) {
       
    }
}

ErrorPtr VulkanEngine::init()
{
    auto error = initFrameBuffers();
    if (error) {
        return error;
    }

    _commandPool = std::make_shared<VulkanCommandPool>(_functionTable, _device);
    _commandPool->init();

    error = initCommandBuffers();
    if (error) {
        return error;
    }

    error = initSemaphores();
    if (error) {
        return error;
    }

    _initialized = true;

    return Error::none();
}

ErrorPtr VulkanEngine::initFrameBuffers()
{
    for(auto& imageView : _swapChain.swapChainImageViews)
    {
        auto framebuffer = std::make_shared<VulkanFrameBuffer>(
            _functionTable,
            _device,
            _graphicsPipeline->renderPass(),
            _swapChain,
            imageView);
        
        auto error = framebuffer->init();
        if (error) {
            return error;
        }


        _swapChainFrameBuffers.push_back(framebuffer);
    }

    return Error::none();
}

ErrorPtr VulkanEngine::initCommandBuffers()
{
    std::cout << "initialize command buffers!" << std::endl;

    _commandBuffers.resize(_swapChain.swapChainImageViews.size());

    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = _commandPool->vulkanHandle();
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t) _commandBuffers.size();

    std::cout << "initialize command buffers!" << std::endl;

    if (_functionTable->AllocateCommandBuffers(_device->vulkanHandle(), &allocInfo, _commandBuffers.data()) != VK_SUCCESS) {
        return Error::create("failed to allocate command buffers!");
    }

    std::cout << "after alloc command buffers!" << std::endl;

    for (size_t i = 0; i < _commandBuffers.size(); i++) {

        std::cout << "command buffer: " << i << std::endl;

        VkCommandBufferBeginInfo beginInfo = {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
        beginInfo.pInheritanceInfo = nullptr; // Optional

        if (_functionTable->BeginCommandBuffer(_commandBuffers[i], &beginInfo) != VK_SUCCESS) {
            return Error::create("failed to begin recording command buffer!");
        }

        std::cout << "command buffer: begin" << i << std::endl;

        VkRenderPassBeginInfo renderPassInfo = {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = _graphicsPipeline->renderPass()->vulkanHandle();
        renderPassInfo.framebuffer = _swapChainFrameBuffers[i]->vulkanHandle();

        std::cout << "command buffer: offset" << i << " " << renderPassInfo.renderPass << " " << renderPassInfo.framebuffer << std::endl;

        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = _swapChain.extent;

        VkClearValue clearColor = {0.0f, 0.0f, 0.0f, 1.0f};
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &clearColor;

        std::cout << "command buffer: begin render" << i << std::endl;

        _functionTable->CmdBeginRenderPass(_commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        std::cout << "command buffer: bind" << i << std::endl;

        _functionTable->CmdBindPipeline(_commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, _graphicsPipeline->vulkanHandle());

        std::cout << "command buffer: draw" << i << std::endl;

        _functionTable->CmdDraw(_commandBuffers[i], 3, 1, 0, 0);

        _functionTable->CmdEndRenderPass(_commandBuffers[i]);

        if (_functionTable->EndCommandBuffer(_commandBuffers[i]) != VK_SUCCESS) {
            return Error::create("failed to record command buffer!");
        }

        std::cout << "command buffer: end" << i << std::endl;
    }

    return Error::none();
}

ErrorPtr VulkanEngine::initSemaphores()
{
    _imageAvailableSemaphore = std::make_shared<VulkanSemaphore>(_functionTable, _device);
    auto error = _imageAvailableSemaphore->init();
    if (error) {
        return error;
    }

    _renderFinishedSemaphore = std::make_shared<VulkanSemaphore>(_functionTable, _device);
    error = _renderFinishedSemaphore->init();
    if (error) {
        return error;
    }

    return Error::none();
}

ErrorPtr VulkanEngine::render()
{
    std::cout << "render" << std::endl;

    uint32_t imageIndex;
    _functionTable->AcquireNextImageKHR(_device->vulkanHandle(), _swapChain.swapChain, std::numeric_limits<uint64_t>::max(), _imageAvailableSemaphore->vulkanHandle(), VK_NULL_HANDLE, &imageIndex);

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {_imageAvailableSemaphore->vulkanHandle()};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &_commandBuffers[imageIndex];

    VkSemaphore signalSemaphores[] = {_renderFinishedSemaphore->vulkanHandle()};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (_functionTable->QueueSubmit(_device->graphicsQueue()->vulkanHandle(), 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS) {
        return Error::create("failed to submit draw command buffer!");
    }

    VkPresentInfoKHR presentInfo = {};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;
    
    VkSwapchainKHR swapChains[] = {_swapChain.swapChain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;
    presentInfo.pResults = nullptr; // Optional

    _functionTable->QueuePresentKHR(_device->presentQueue()->vulkanHandle(), &presentInfo);

    std::cout << "present" << std::endl;

    return Error::none();
}
