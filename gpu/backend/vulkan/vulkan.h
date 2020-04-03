#ifndef VULKAN_H
#define VULKAN_H

#include "graphics/graphics-backend.h"
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
    void checkValidationLayerSupport(const std::vector<const char*>& requiredLayers);

    std::shared_ptr<VulkanFunctionTable> _vk;

    VkInstance _instance;

    VulkanPhysicalDevicePtr _physicalDevice;
    VulkanDevicePtr _device;

    VkDebugReportCallbackEXT _callback;

    // std::vector<VulkanWindow> m_windows;
};

#endif
