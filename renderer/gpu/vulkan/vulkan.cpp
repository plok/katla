#include "vulkan.h"

#include "window/window-factory.h"
#include "gpu/vulkan/vulkan-window-factory.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vulkan/vulkan.h>

#include "string.h"
#include <iostream>
#include <vector>

#include <sstream>

void glfw_vulkan_error_callback(int error, const char* description)
{
    std::cout << description << std::endl;
}

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
    DEFINE_VFO(GetPhysicalDeviceQueueFamilyProperties)
    DEFINE_VFO(GetDeviceProcAddr)
    DEFINE_VFO(EnumerateDeviceExtensionProperties)
    DEFINE_VFO(CreateDevice)
    DEFINE_VFO(EnumeratePhysicalDevices)
    DEFINE_VFO(GetPhysicalDeviceProperties)
    DEFINE_VFO(GetPhysicalDeviceFeatures)
    DEFINE_VFO(DestroyInstance)
};

Vulkan::Vulkan() :
    m_instance(nullptr),
    m_functionTable(nullptr)
{
}

Vulkan::~Vulkan()
{
}

ErrorPtr Vulkan::init()
{
    if (!glfwInit()) {
        return Error::create("Failed initializing GLFW!");
    }

    glfwSetErrorCallback(glfw_vulkan_error_callback);
    
    if (!glfwVulkanSupported()) {
        return Error::create("No vulkan supported!");
    }

    m_functionTable = std::make_unique<VulkanFunctionTable>();
   


    uint32_t count;
    const char** extensions = glfwGetRequiredInstanceExtensions(&count);

    std::cout << count << " extensions supported" << std::endl;

    VkApplicationInfo appInfo;
    memset(&appInfo, 0, sizeof(appInfo));
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "katla";
    appInfo.applicationVersion = 1;
    appInfo.pEngineName = "katla";
    appInfo.engineVersion = 1;
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo instanceInfo;
    memset(&instanceInfo, 0, sizeof(instanceInfo));
    instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceInfo.enabledExtensionCount = count;
    instanceInfo.ppEnabledExtensionNames = extensions;
    instanceInfo.pApplicationInfo = &appInfo;

    VkResult res;

    res = m_functionTable->CreateInstance(&instanceInfo, NULL, &m_instance);
    if (res == VK_ERROR_INCOMPATIBLE_DRIVER) {

        return Error::create("cannot find a compatible Vulkan ICD");

    } else if (res) {
        return Error::create("unknown error");
    }

    uint32_t gpuCount = 1;

    // Get number of devices
    res = m_functionTable->EnumeratePhysicalDevices(m_instance, &gpuCount, NULL);

    if (gpuCount == 0) {
        return Error::create("No gpu found");
    }

    std::vector<VkPhysicalDevice> physicalDevices (gpuCount);

    res = m_functionTable->EnumeratePhysicalDevices(m_instance, &gpuCount, physicalDevices.data());

    for (auto physicalDevice : physicalDevices) {
        // Return info from device
        VkPhysicalDeviceProperties deviceProperties;
        VkPhysicalDeviceFeatures   deviceFeatures;

        m_functionTable->GetPhysicalDeviceProperties( physicalDevice, &deviceProperties );
        m_functionTable->GetPhysicalDeviceFeatures( physicalDevice, &deviceFeatures );

        uint32_t majorVersion = VK_VERSION_MAJOR( deviceProperties.apiVersion );
        uint32_t minorVersion = VK_VERSION_MINOR( deviceProperties.apiVersion );
        uint32_t patchVersion = VK_VERSION_PATCH( deviceProperties.apiVersion );
        
        std::cout << "Found device: " << deviceProperties.deviceID << " - " << deviceProperties.deviceType << " - " << deviceProperties.deviceName << std::endl;

        if( (majorVersion < 1) &&
            (deviceProperties.limits.maxImageDimension2D < 4096) ) {

            std::stringstream message; // ugly
            message << "Physical device " << physicalDevice << " doesn't support required parameters!";
            return Error::create(message.str());
        }

        // if (glfwGetPhysicalDevicePresentationSupport(instance, physicalDevice, queue_family_index))
        // {
        //     // Queue family supports image presentation
        // }
        
    }


    return Error::none();
}

void Vulkan::cleanup()
{
    if (m_instance) {
        m_functionTable->DestroyInstance(m_instance, NULL);
        m_instance = nullptr;
    }

    glfwTerminate();
}

std::unique_ptr<WindowFactory> Vulkan::windowFactory()
{
    return std::make_unique<VulkanWindowFactory>(m_instance);
}