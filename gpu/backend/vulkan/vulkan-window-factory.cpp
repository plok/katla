#include "vulkan-window-factory.h"
#include "vulkan-window.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vulkan/vulkan.h>

#include "vulkan-function-table.h"

VulkanWindowFactory::VulkanWindowFactory(std::shared_ptr<VulkanFunctionTable> functionTable, VkInstance instance, VulkanPhysicalDevicePtr physicalDevice, VulkanDevicePtr device) :
    m_functionTable(functionTable),
    m_instance(instance),
    m_physicalDevice(physicalDevice),
    m_device(device)
{}

std::tuple<WindowPtr, ErrorPtr> VulkanWindowFactory::create(std::shared_ptr<RenderView>  /*renderView*/, std::shared_ptr<WindowProperties> properties)
{
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow* window = glfwCreateWindow(properties->size.width, properties->size.height, properties->title.c_str(), nullptr, nullptr);
    if (!window) {
        return {std::shared_ptr<Window>(), Error::create("Error creating window")};
    }

    VkSurfaceKHR surface;
    VkResult err = glfwCreateWindowSurface(m_instance, window, nullptr, &surface);
    if (err)
    {
        glfwDestroyWindow(window);

        // Window surface creation failed
        return {std::shared_ptr<Window>(), Error::create("Error creating window surface")};
    }

    auto swapChain = createSwapChain(surface, properties);

    auto vulkanWindow = std::make_shared<VulkanWindow>(m_functionTable, m_device, window, surface, swapChain);

    return {vulkanWindow, Error::none()};    
}

// struct SwapChainSupportDetails {
//     VkSurfaceCapabilitiesKHR capabilities;
//     std::vector<VkSurfaceFormatKHR> formats;
//     std::vector<VkPresentModeKHR> presentModes;
// };

VkSwapchainKHR VulkanWindowFactory::createSwapChain(VkSurfaceKHR surface, std::shared_ptr<WindowProperties> properties)
{
    // SwapChainSupportDetails details;

    // uint32_t formatCount;
    // vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

    // if (formatCount != 0) {
    //     details.formats.resize(formatCount);
    //     vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
    // }

    // uint32_t presentModeCount;
    // vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

    // if (presentModeCount != 0) {
    //     details.presentModes.resize(presentModeCount);
    //     vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
    // }

    auto surfaceFormat = chooseSurfaceFormat(surface);

    VkSurfaceCapabilitiesKHR capabilities;
    m_functionTable->GetPhysicalDeviceSurfaceCapabilitiesKHR(m_physicalDevice->vulkanHandle(), surface, &capabilities);

    auto extent = chooseSwapExtent(surface, capabilities, properties);

    uint32_t imageCount = capabilities.minImageCount + 1;
    if (capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount) {
        imageCount = capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface;
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    // TODO: specify concurrent when we have multiple queues

    // QueueFamilyIndices indices = findQueueFamilies(physicalDevice);
    // uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    // if (indices.graphicsFamily != indices.presentFamily) {
    //     createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
    //     createInfo.queueFamilyIndexCount = 2;
    //     createInfo.pQueueFamilyIndices = queueFamilyIndices;
    // } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0; // Optional
        createInfo.pQueueFamilyIndices = nullptr; // Optional
    // }

    createInfo.preTransform = capabilities.currentTransform;

    // TODO we want to use alpha for the compositor??
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

    auto swapPresentMode = chooseSwapPresentMode(surface);
    createInfo.presentMode = swapPresentMode;
    createInfo.clipped = VK_TRUE;

    // TODO we need to handle this when resizing windows
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    VkSwapchainKHR swapChain;
    if (m_functionTable->CreateSwapchainKHR(m_device->vulkanHandle(), &createInfo, nullptr, &swapChain) != VK_SUCCESS) {
        throw std::runtime_error("failed to create swap chain!");
    }

    return swapChain;
}

VkSurfaceFormatKHR VulkanWindowFactory::chooseSurfaceFormat(VkSurfaceKHR surface)
{
    std::vector<VkSurfaceFormatKHR> formats;

    // TODO check for surface formats when selecting device and dont do it twice

    uint32_t formatCount;
    m_functionTable->GetPhysicalDeviceSurfaceFormatsKHR(m_physicalDevice->vulkanHandle(), surface, &formatCount, nullptr);

    if (formatCount != 0) {
        formats.resize(formatCount);
        m_functionTable->GetPhysicalDeviceSurfaceFormatsKHR(m_physicalDevice->vulkanHandle(), surface, &formatCount, formats.data());
    }

    if (formats.size() == 1 && formats[0].format == VK_FORMAT_UNDEFINED) {
        return {VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
    }

    for (const auto& availableFormat : formats) {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return availableFormat;
        }
    }

    return formats[0];
}

VkPresentModeKHR VulkanWindowFactory::chooseSwapPresentMode(VkSurfaceKHR surface)
{
    std::vector<VkPresentModeKHR> presentModes;
    VkPresentModeKHR bestMode = VK_PRESENT_MODE_FIFO_KHR;

    uint32_t presentModeCount;
    m_functionTable->GetPhysicalDeviceSurfacePresentModesKHR(m_physicalDevice->vulkanHandle(), surface, &presentModeCount, nullptr);

    if (presentModeCount != 0) {
        presentModes.resize(presentModeCount);
        m_functionTable->GetPhysicalDeviceSurfacePresentModesKHR(m_physicalDevice->vulkanHandle(), surface, &presentModeCount, presentModes.data());
    }

    for (const auto& availablePresentMode : presentModes) {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return availablePresentMode;
        } else if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
            bestMode = availablePresentMode;
        }
    }

    return bestMode;
}

VkExtent2D VulkanWindowFactory::chooseSwapExtent(VkSurfaceKHR surface, const VkSurfaceCapabilitiesKHR& capabilities, std::shared_ptr<WindowProperties> properties)
{
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
    } else {
        VkExtent2D actualExtent = {static_cast<uint32_t>(properties->size.width), static_cast<uint32_t>(properties->size.height)};

        actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
        actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

        return actualExtent;
    }
}
