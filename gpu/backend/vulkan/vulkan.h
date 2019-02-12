#ifndef VULKAN_H
#define VULKAN_H

#include "gpu/backend/graphics-backend.h"
#include "common/error.h"

// TODO non-vulkan interfaces
#include "gpu/backend/vulkan/vulkan-physical-device.h"
#include "gpu/backend/vulkan/vulkan-device.h"
#include "gpu/backend/vulkan/vulkan-window.h"

#include <vulkan/vulkan_core.h>

#include <memory>

class WindowFactory;

class VulkanFunctionTable;

class Vulkan : public GraphicsBackend
{
public:
    Vulkan();
    virtual ~Vulkan();

    ErrorPtr init();
    void cleanup();

    ErrorPtr initDevice();

    std::tuple<VulkanPhysicalDevicePtr, ErrorPtr> selectDevice();
    std::tuple<VulkanDevicePtr, ErrorPtr> initDevice(VulkanPhysicalDevicePtr physicalDevice);

    std::unique_ptr<WindowFactory> windowFactory();
    
private:
    std::shared_ptr<VulkanFunctionTable> _vk;

    VkInstance _instance;

    VulkanPhysicalDevicePtr _physicalDevice;
    VulkanDevicePtr _device;

    // std::vector<VulkanWindow> m_windows;
};

#endif
