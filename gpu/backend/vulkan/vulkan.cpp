#include "vulkan.h"

#include "window/window-factory.h"

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

Vulkan::Vulkan() :
    m_instance(nullptr),
    m_functionTable(nullptr)
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

    m_functionTable = std::make_shared<VulkanFunctionTable>();

    uint32_t count;
    const char** extensions = glfwGetRequiredInstanceExtensions(&count);

    for (uint32_t i=0; i<count; i++)
        std::cout << "glfw required: " << extensions[i] << std::endl;

    std::cout << count << " extensions supported" << std::endl;

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
    instanceInfo.enabledExtensionCount = count;
    instanceInfo.ppEnabledExtensionNames = extensions;
    instanceInfo.pApplicationInfo = &appInfo;

    VkResult res;

    res = m_functionTable->CreateInstance(&instanceInfo, nullptr, &m_instance);
    if (res == VK_ERROR_INCOMPATIBLE_DRIVER) {
        return Error::create("cannot find a compatible Vulkan ICD");
    }
    if (res) {
        return Error::create("unknown error");
    }

    return Error::none();
}

void Vulkan::cleanup()
{
    if (m_instance) {
        m_functionTable->DestroyInstance(m_instance, nullptr);
        m_instance = nullptr;
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
    m_physicalDevice = physicalDevice;

    auto [device, initError] = initDevice(m_physicalDevice);

    if (initError) {
        return initError;
    }
    m_device = device;

    return Error::none();
}

std::tuple<VulkanPhysicalDevicePtr, ErrorPtr> Vulkan::selectDevice()
{
    PhysicalDeviceFactory physicalDeviceFactory(m_functionTable, m_instance);
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
    DeviceFactory deviceFactory(m_instance, m_functionTable);
    return deviceFactory.create(physicalDevice);
}

std::unique_ptr<WindowFactory> Vulkan::windowFactory()
{
    // TODO possible segfault because of uninitialized devices
    return std::make_unique<VulkanWindowFactory>(m_functionTable, m_instance, m_physicalDevice, m_device);
}
