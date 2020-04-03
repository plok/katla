#include "vulkan.h"

#include "window-management/window-factory.h"

#include "gpu/backend/vulkan/vulkan-function-table.h"
#include "gpu/backend/vulkan/vulkan-window-factory.h"

#include "gpu/backend/vulkan/vulkan-physical-device.h"
#include "gpu/backend/vulkan/vulkan-physical-device-factory.h"
#include "gpu/backend/vulkan/vulkan-device.h"
#include "gpu/backend/vulkan/vulkan-device-factory.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vulkan/vulkan.h>

#include <set>
#include <cstring>
#include <iostream>
#include <utility>
#include <vector>

#include <sstream>

void glfw_vulkan_error_callback(int  /*error*/, const char* description)
{
    std::cout << description << std::endl;
}

VKAPI_ATTR VkBool32 VKAPI_CALL DebugReportCallback(
    VkDebugReportFlagsEXT       flags,
    VkDebugReportObjectTypeEXT  objectType,
    uint64_t                    object,
    size_t                      location,
    int32_t                     messageCode,
    const char*                 pLayerPrefix,
    const char*                 pMessage,
    void*                       pUserData)
{
    std::cerr << pMessage << std::endl;
    return VK_FALSE;
}

Vulkan::Vulkan() :
    _vk(nullptr),
    _instance(nullptr),
    _device(nullptr)
{
}

Vulkan::~Vulkan() = default;

ErrorPtr Vulkan::init()
{
    if (!glfwInit()) {
        return Error::create("Failed initializing GLFW!");
    }

    glfwSetErrorCallback(glfw_vulkan_error_callback);
    
    if (!glfwVulkanSupported()) {
        return Error::create("No vulkan supported!");
    }

    _vk = std::make_shared<VulkanFunctionTable>();


    std::vector<const char*> enabledInstanceLayers;
    std::vector<const char*> enabledInstanceExtensions;

#ifndef NDEBUG
    enabledInstanceLayers.push_back("VK_LAYER_LUNARG_standard_validation");
    enabledInstanceExtensions.push_back("VK_EXT_debug_report");
#endif

    uint32_t count;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&count);

    for (uint32_t i=0; i<count; i++) {
        enabledInstanceExtensions.push_back(glfwExtensions[i]);
        std::cout << "glfw required: " << glfwExtensions[i] << std::endl;
    }

    std::cout << count << " extensions supported" << std::endl;

    checkValidationLayerSupport(enabledInstanceLayers);

    VkApplicationInfo appInfo;
    memset(&appInfo, 0, sizeof(appInfo));
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "katla";
    appInfo.applicationVersion = 1;
    appInfo.pEngineName = "katla";
    appInfo.engineVersion = 1;
    appInfo.apiVersion = VK_API_VERSION_1_1;

    VkInstanceCreateInfo instanceInfo;
    memset(&instanceInfo, 0, sizeof(instanceInfo));
    instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceInfo.enabledLayerCount = static_cast<uint32_t>(enabledInstanceLayers.size());
    instanceInfo.ppEnabledLayerNames = &enabledInstanceLayers[0];
    instanceInfo.enabledExtensionCount = static_cast<uint32_t>(enabledInstanceExtensions.size());
    instanceInfo.ppEnabledExtensionNames = &enabledInstanceExtensions[0];
    instanceInfo.pApplicationInfo = &appInfo;

    VkResult res;

    res = _vk->CreateInstance(&instanceInfo, nullptr, &_instance);
    if (res == VK_ERROR_INCOMPATIBLE_DRIVER) {
        return Error::create("cannot find a compatible Vulkan ICD");
    }
    if (res) {
        return Error::create("unknown error");
    }

#ifndef NDEBUG
    VkDebugReportCallbackCreateInfoEXT callbackCreateInfo;
    callbackCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
    callbackCreateInfo.pNext = nullptr;
    callbackCreateInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT |
                               VK_DEBUG_REPORT_WARNING_BIT_EXT |
                               VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
    callbackCreateInfo.pfnCallback = &DebugReportCallback;
    callbackCreateInfo.pUserData = nullptr;

    // #ifndef NDEBUG
    //     DEFINE_VFO(CreateDebugReportCallbackEXT)
    //     DEFINE_VFO(DebugReportMessageEXT)
    //     DEFINE_VFO(DestroyDebugReportCallbackEXT)
    // #endif

    PFN_vkCreateDebugReportCallbackEXT createDebugReportCallbackEXT =
        reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>(
            _vk->GetInstanceProcAddr(_instance,
                                     "vkCreateDebugReportCallbackEXT"));

    VkResult result = createDebugReportCallbackEXT(
        _instance, &callbackCreateInfo, nullptr, &_callback);
#endif

     return Error::none();
}

void Vulkan::cleanup()
{
    if (_instance) {
        _vk->DestroyInstance(_instance, nullptr);
        _instance = nullptr;
    }

    glfwTerminate();
}

ErrorPtr Vulkan::initDevice()
{
    auto [physicalDevice, selectError] = selectDevice();

    std::cout << "Selected device: " << physicalDevice->name() << std::endl;

    if (selectError) {
        return selectError;
    }
    _physicalDevice = physicalDevice;

    auto [device, initError] = initDevice(_physicalDevice);

    if (initError) {
        return initError;
    }
    _device = device;

    return Error::none();
}

std::tuple<VulkanPhysicalDevicePtr, ErrorPtr> Vulkan::selectDevice()
{
    PhysicalDeviceFactory physicalDeviceFactory(*_vk, _instance);
    auto [physicalDevices, getDeviceError] = physicalDeviceFactory.getPhysicalDevices();

    if (getDeviceError) {
        return {VulkanPhysicalDevicePtr(), getDeviceError};
    }

    VulkanPhysicalDevicePtr selectedPhysicalDevice;
    for (const auto& physicalDevice : physicalDevices) {
        
        // TODO score devices;

        physicalDevice->printInfo();

        auto [deviceExtensions, getDeviceExtensionsError] = physicalDevice->getExtensions();        
        for (auto ext: deviceExtensions) {
            std::cout << "Supported device extensions: " << static_cast<char*>(ext.extensionName) << std::endl;
        }

        // TODO: create single default value somewhere and allow user to change
        std::set<std::string> device_extension_names(
            {VK_KHR_EXTERNAL_MEMORY_EXTENSION_NAME, VK_KHR_SWAPCHAIN_EXTENSION_NAME}
        );

        for (const auto& extension : deviceExtensions) {
            device_extension_names.erase(std::string(extension.extensionName));
        }

        if (!device_extension_names.empty())
        {
            std::cout << " Missing extensions: ";
            for (const auto& extension : device_extension_names) {
                std::cout << extension << ",";
            }
            std::cout << std::endl;
        }

        auto queueFamilyProperties = physicalDevice->getQueueFamilies();
        std::cout << "QueueFamilies count: " << queueFamilyProperties.size() << std::endl;

        // TODO validate if device can output to surface?
//        auto error = physicalDevice->validateForGraphics(surface);
//        if (!error) {
           selectedPhysicalDevice = physicalDevice;
//        }
    }

    if (!selectedPhysicalDevice) {
        return {VulkanPhysicalDevicePtr(), Error::create("no device found!")};
    }

    return {selectedPhysicalDevice, Error::none()};
}

std::tuple<VulkanDevicePtr, ErrorPtr> Vulkan::initDevice(VulkanPhysicalDevicePtr physicalDevice)
{
    DeviceFactory deviceFactory(*_vk, _instance);
    return deviceFactory.create(physicalDevice);
}

std::unique_ptr<WindowFactory> Vulkan::windowFactory()
{
    // TODO possible segfault because of uninitialized devices
    return std::make_unique<VulkanWindowFactory>(*_vk, _instance, *_device, *_physicalDevice);
}

void Vulkan::checkValidationLayerSupport(const std::vector<const char*>& requiredLayers)
{
    uint32_t layerCount;
    _vk->EnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    _vk->EnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    std::vector<std::string> layerSet(availableLayers.size());
    for(auto layer : availableLayers) {
        layerSet.push_back(layer.layerName);

        std::cout << "AvailableLayers: " << layer.layerName << std::endl;
    }

    for(auto layer : requiredLayers)
    {
        auto found = std::find(layerSet.begin(), layerSet.end(), layer);
        if (found == layerSet.end())
        {
            std::cerr << "Required layer not found: " << layer << std::endl;
        }
    }
}
