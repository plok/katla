#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include "string.h"
#include <iostream>
#include <vector>


void glfw_error_callback(int error, const char* description)
{
    std::cout << description << std::endl;
}

int main() {
    glfwInit();

    if (!glfwVulkanSupported())
    {
        std::cout << "No vulkan supported!";
        return -1;
    }

    glfwSetErrorCallback(glfw_error_callback);

    PFN_vkCreateInstance createInstance =
        (PFN_vkCreateInstance) glfwGetInstanceProcAddress(NULL, "vkCreateInstance");

   // VK_INSTANCE_LEVEL_FUNCTION( vkGetPhysicalDeviceQueueFamilyProperties )
   // VK_INSTANCE_LEVEL_FUNCTION( vkGetDeviceProcAddr )
   // VK_INSTANCE_LEVEL_FUNCTION( vkEnumerateDeviceExtensionProperties )

    auto createDevice =
        (PFN_vkCreateDevice) glfwGetInstanceProcAddress(NULL, "vkCreateDevice");
    auto vkEnumeratePhysicalDevices =
        (PFN_vkEnumeratePhysicalDevices) glfwGetInstanceProcAddress(NULL, "vkEnumeratePhysicalDevices");
    auto vkGetPhysicalDeviceProperties =
        (PFN_vkGetPhysicalDeviceProperties) glfwGetInstanceProcAddress(NULL, "vkGetPhysicalDeviceProperties");
    auto vkGetPhysicalDeviceFeatures =
        (PFN_vkGetPhysicalDeviceFeatures) glfwGetInstanceProcAddress(NULL, "vkGetPhysicalDeviceFeatures");
    auto vkDestroyInstance =
        (PFN_vkDestroyInstance) glfwGetInstanceProcAddress(NULL, "vkDestroyInstance");

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

    VkInstance instance;
    VkResult res;
    
    res = createInstance(&instanceInfo, NULL, &instance);
    if (res == VK_ERROR_INCOMPATIBLE_DRIVER) {
        std::cout << "cannot find a compatible Vulkan ICD\n";
        exit(-1);
    } else if (res) {
        std::cout << "unknown error\n";
        exit(-1);
    }
    
    uint32_t gpuCount = 1;

    // Get number of devices
    res = vkEnumeratePhysicalDevices(instance, &gpuCount, NULL);

    if (gpuCount == 0) {
        std::cout << "no gpu found!";
        return -1;
    }

    std::vector<VkPhysicalDevice> physicalDevices (gpuCount);

    res = vkEnumeratePhysicalDevices(instance, &gpuCount, physicalDevices.data());


   
    
    for (auto physicalDevice : physicalDevices) {
        // Return info from device
        VkPhysicalDeviceProperties deviceProperties;
        VkPhysicalDeviceFeatures   deviceFeatures;

        vkGetPhysicalDeviceProperties( physicalDevice, &deviceProperties );
        vkGetPhysicalDeviceFeatures( physicalDevice, &deviceFeatures );

        uint32_t majorVersion = VK_VERSION_MAJOR( deviceProperties.apiVersion );
        uint32_t minorVersion = VK_VERSION_MINOR( deviceProperties.apiVersion );
        uint32_t patchVersion = VK_VERSION_PATCH( deviceProperties.apiVersion );
         
        std::cout << "Found device: " << deviceProperties.deviceID << " - " << deviceProperties.deviceType << " - " << deviceProperties.deviceName << std::endl;

        if( (majorVersion < 1) &&
            (deviceProperties.limits.maxImageDimension2D < 4096) ) {
          std::cout << "Physical device " << physicalDevice << " doesn't support required parameters!" << std::endl;
          return false;
        }

        // if (glfwGetPhysicalDevicePresentationSupport(instance, physicalDevice, queue_family_index))
        // {
        //     // Queue family supports image presentation
        // }
        
    }

    

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow* window = glfwCreateWindow(640, 480, "Window Title", NULL, NULL);

    glm::mat4 matrix;
    glm::vec4 vec;
    auto test = matrix * vec;

    VkSurfaceKHR surface;
    VkResult err = glfwCreateWindowSurface(instance, window, NULL, &surface);
    if (err)
    {
        // Window surface creation failed
        std::cout << "Error creating surface!";
    }

    while(!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }

    glfwDestroyWindow(window);


    vkDestroyInstance(instance, NULL);


    glfwTerminate();

    return 0;
}

