#include "vulkan-device-queue.h"

#include <utility>

#include "vulkan.h"
#include "vulkan-function-table.h"

DeviceQueue::DeviceQueue(
        VulkanFunctionTable& vk,
        VkQueue queue,
        int queueIndex) :
    _vk(vk),
    m_queue(queue),
    m_queueIndex(queueIndex)
{
}

DeviceQueue::~DeviceQueue() = default;
