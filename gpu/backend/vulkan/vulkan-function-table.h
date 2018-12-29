#ifndef VULKAN_FUNCTION_TABLE_H
#define VULKAN_FUNCTION_TABLE_H

#include "common/error.h"

#include <vulkan/vulkan_core.h>

// TODO remove glfw3 dependency
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>

class VulkanFunctionTable
{
public:
    /**
     * Vulkan Proc handles
     */

     // TODO not much safer than simple macro without using a function object

    #define DEFINE_VFO(name) class VFO_##name \
    { \
    public: \
        VFO_##name () : \
            m_functionPointer(nullptr) \
        { \
            std::cout << "vk" #name << std::endl; \
            m_functionPointer = ( PFN_vk##name ) glfwGetInstanceProcAddress(NULL, "vk" #name); \
            std::cout << m_functionPointer << std::endl; \
        } \
        operator PFN_vk##name() const { return m_functionPointer; } \
    private:    \
        PFN_vk##name m_functionPointer; \
    }; \
    VFO_##name name;
    
    DEFINE_VFO(CreateInstance)
    DEFINE_VFO(DestroyInstance)

    DEFINE_VFO(GetDeviceProcAddr)
    DEFINE_VFO(EnumerateDeviceExtensionProperties)

    DEFINE_VFO(EnumeratePhysicalDevices)
    DEFINE_VFO(GetPhysicalDeviceProperties)
    DEFINE_VFO(GetPhysicalDeviceFeatures)
    DEFINE_VFO(GetPhysicalDeviceQueueFamilyProperties)
    DEFINE_VFO(GetPhysicalDeviceSurfaceSupportKHR)
    DEFINE_VFO(GetPhysicalDeviceSurfaceFormatsKHR)
    DEFINE_VFO(GetPhysicalDeviceSurfacePresentModesKHR)
    DEFINE_VFO(GetPhysicalDeviceSurfaceCapabilitiesKHR)

    DEFINE_VFO(CreateDevice)
    DEFINE_VFO(GetDeviceQueue)

    DEFINE_VFO(CreateSwapchainKHR)
    DEFINE_VFO(DestroySwapchainKHR)
};

#endif