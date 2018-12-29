#ifndef VULKAN_WINDOW_FACTORY_H
#define VULKAN_WINDOW_FACTORY_H

#include "gpu/backend/vulkan/vulkan-window.h"
#include "gpu/backend/vulkan/vulkan-physical-device.h"

#include "window/window.h"
#include "window/window-factory.h"
#include "common/error.h"

#include "gpu/render/render-view.h"

#include <memory>
#include <string>

#include <vulkan/vulkan.h>

class WindowScene;

class VulkanWindowFactory : public WindowFactory {
public:
    // Takes ownership
    VulkanWindowFactory(std::shared_ptr<VulkanFunctionTable> functionTable, VkInstance instance, VulkanPhysicalDevicePtr physicalDevice, VulkanDevicePtr device);

    std::tuple<WindowPtr, ErrorPtr> create(std::shared_ptr<RenderView> renderView, std::shared_ptr<WindowProperties> properties);

private:
    VkSwapchainKHR createSwapChain(VkSurfaceKHR surface, std::shared_ptr<WindowProperties> properties);
    VkSurfaceFormatKHR chooseSurfaceFormat(VkSurfaceKHR surface);
    VkPresentModeKHR chooseSwapPresentMode(VkSurfaceKHR surface);
    VkExtent2D chooseSwapExtent(VkSurfaceKHR surface, const VkSurfaceCapabilitiesKHR& capabilities, std::shared_ptr<WindowProperties> properties);

    VkInstance m_instance;
    std::shared_ptr<VulkanFunctionTable> m_functionTable;

    // TODO: move to window?, as there doesn't have to be a single device for all windows
    VulkanPhysicalDevicePtr m_physicalDevice;
    VulkanDevicePtr m_device;
};

#endif