#include "vulkan-device-queue.h"

#include <utility>

#include "vulkan.h"
#include "vulkan-function-table.h"

DeviceQueue::DeviceQueue(std::shared_ptr<VulkanFunctionTable> vft, VkQueue queue, int queueIndex) :
    m_functionTable(std::move(vft)),
    m_queue(queue),
    m_queueIndex(queueIndex)
{
}

DeviceQueue::~DeviceQueue() = default;
