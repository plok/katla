// Copyright(c) 2018, NVIDIA CORPORATION. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// VulkanHpp Samples : 02_EnumerateDevices
//                     Enumerate physical devices

#include "vulkan/vulkan.hpp"
#include <iostream>

static char const *AppName = "02_EnumerateDevices";
static char const *EngineName = "Vulkan.hpp";

int main(int argc, char *argv[]) {
    try {

        auto instanceExtensionProperties = vk::enumerateInstanceExtensionProperties();

        for (auto property:  instanceExtensionProperties) {
            std::cout << "Supported extensions:" << property.extensionName << std::endl;
        }

        std::vector<const char *> extension_names({VK_KHR_DISPLAY_EXTENSION_NAME, VK_KHR_SURFACE_EXTENSION_NAME});

        vk::Instance result1;
        auto const appInfo = vk::ApplicationInfo()
                .setPApplicationName(AppName)
                .setApplicationVersion(0)
                .setPEngineName(EngineName)
                .setEngineVersion(0)
                .setApiVersion(VK_API_VERSION_1_1);
        auto const inst_info = vk::InstanceCreateInfo()
                .setPApplicationInfo(&appInfo)
                .setEnabledExtensionCount(extension_names.size())
                .setPpEnabledExtensionNames(extension_names.data());


        vk::Instance instance1 = createInstance(inst_info, nullptr);
        result1= instance1;
        vk::Instance instance = result1;


        printf("Have instance!\n");

        vk::PhysicalDevice result;
        auto physicalDevices = instance.enumeratePhysicalDevices();
        printf("size: %zu\n", physicalDevices.size());

        vk::PhysicalDevice gpu1 = physicalDevices[1];
        result= gpu1;
        vk::PhysicalDevice gpu = result;

        printf("gpu: %s\n", gpu.getProperties().deviceName);


        /* Look for device extensions */
        std::vector<const char *> device_extension_names(
                {VK_KHR_EXTERNAL_MEMORY_EXTENSION_NAME, VK_KHR_SWAPCHAIN_EXTENSION_NAME});
        auto extensionProperties = gpu.enumerateDeviceExtensionProperties();
        for (auto i: extensionProperties) {
            printf("Supperted device extensions: %s\n", i.extensionName);
        }


        /* Layers */
        std::vector<const char *> enabledLayers({"VK_LAYER_LUNARG_standard_validation"});

        vk::DeviceCreateInfo result2;
        auto layerProperties = vk::enumerateInstanceLayerProperties();
        for (auto layerProperty : layerProperties) {
            printf("layer property supported: %s \n", layerProperty.layerName);
        }


        auto queueFamilyProperties = gpu.getQueueFamilyProperties();

        uint32_t graphicsQueueFamilyIndex = 0;
        for (size_t i1 = 0; i1 < queueFamilyProperties.size(); i1++) {
            if (queueFamilyProperties[i1].queueFlags & vk::QueueFlagBits::eGraphics) {
                graphicsQueueFamilyIndex = i1;
                break;
            }
        }

        printf("Selected graphicsQueue, %d of %zu \n", graphicsQueueFamilyIndex, queueFamilyProperties.size());
        float priorities[] = {0.0f};
        auto queueCreateInfo = vk::DeviceQueueCreateInfo()
                .setPNext(NULL)
                .setQueueCount(1)
                .setQueueFamilyIndex(graphicsQueueFamilyIndex)
                .setPQueuePriorities(priorities);

        std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos({queueCreateInfo});
        auto deviceInfo1 = vk::DeviceCreateInfo()
                .setPNext(NULL)
                .setEnabledLayerCount(enabledLayers.size())
                .setPpEnabledLayerNames(enabledLayers.data())
                .setPQueueCreateInfos(&queueCreateInfo)
                .setEnabledExtensionCount(device_extension_names.size())
                .setPpEnabledExtensionNames(device_extension_names.data());
        printf("Creating device \n");
        result2= deviceInfo1;
        vk::DeviceCreateInfo deviceInfo = result2;


        vk::Device device = gpu.createDevice(deviceInfo);

        printf("Device created");

        auto displayPropertiesList = gpu.getDisplayPropertiesKHR();
        auto propertiesKHR = displayPropertiesList.front();

        printf("Have displayProperties");
        const auto display = propertiesKHR.display;

        auto displayModePropertiesKHR = gpu.getDisplayModePropertiesKHR(display);

        auto displayPlanePropertiesKHR = gpu.getDisplayPlanePropertiesKHR();

        vk::DisplayPlanePropertiesKHR selectedPlane;
        uint32_t index = 0;
        for (auto plane : displayPlanePropertiesKHR) {
            if (plane.currentDisplay && plane.currentDisplay != display) {
                continue;
            }
            auto supportedDisplaysKHR = gpu.getDisplayPlaneSupportedDisplaysKHR(plane.currentStackIndex);
            if (supportedDisplaysKHR.size() > 0) {
                selectedPlane = plane;
                printf("yoohoooo\n");
                index++;
            }

        }


        auto const createInfo = vk::DisplaySurfaceCreateInfoKHR()
                .setDisplayMode(displayModePropertiesKHR.front().displayMode)
                .setPlaneIndex(index)
                .setPlaneStackIndex(selectedPlane.currentStackIndex)
                .setGlobalAlpha(1.0f);
        auto surfaceKHR = instance.createDisplayPlaneSurfaceKHR(createInfo);
        if (surfaceKHR) {
            printf("I have many successes\n");
        }


        auto surfaceCapabilitiesKHR = gpu.getSurfaceCapabilitiesKHR(surfaceKHR);

        VkExtent2D swapchainExtent = {};
        if (surfaceCapabilitiesKHR.currentExtent.width == -1 || surfaceCapabilitiesKHR.currentExtent.height == -1) {
            printf("oh noo");
        } else {
            swapchainExtent = surfaceCapabilitiesKHR.currentExtent;
        }


        auto presentModesKHR = gpu.getSurfacePresentModesKHR(surfaceKHR);

        auto presentMode1 = vk::PresentModeKHR::eFifo;
        printf("presentModes: %d", presentModesKHR.size());
        for (auto mode : presentModesKHR) {
            if (mode == vk::PresentModeKHR::eMailbox) {
                presentMode1 = vk::PresentModeKHR::eMailbox;
                printf("Mailbox presentmode \n");
                break;
            }

            if (mode == vk::PresentModeKHR::eImmediate) {
                presentMode1 = vk::PresentModeKHR::eImmediate;
                printf("Immediate presentmode \n");

            }
        }
        vk::PresentModeKHR presentMode = presentMode1;



        auto imageCount = surfaceCapabilitiesKHR.minImageCount + 1;
        if (imageCount > surfaceCapabilitiesKHR.maxImageCount)
            imageCount = surfaceCapabilitiesKHR.maxImageCount;

        auto surfaceFormatsKHR = gpu.getSurfaceFormatsKHR(surfaceKHR);
        vk::Format format;
        if (surfaceFormatsKHR.size() == 1 && surfaceFormatsKHR.front().format == vk::Format::eUndefined) {
            format = vk::Format::eB8G8R8A8Unorm;
        }
        auto colorspace = surfaceFormatsKHR.front().colorSpace;


        auto swapchainCreateInfoKHR = vk::SwapchainCreateInfoKHR()
                .setMinImageCount(imageCount)
                .setSurface(surfaceKHR)
                .setImageColorSpace(colorspace)
                .setImageFormat(format)
                .setImageArrayLayers(1)
                .setPresentMode(presentMode)
                .setClipped(true);

        auto swapchainKHR = device.createSwapchainKHR(swapchainCreateInfoKHR);
        if (swapchainKHR) {
            printf("Pure awesomeness");
        }



        /* VULKAN_HPP_KEY_END */
    } catch (vk::SystemError err) {
        std::cout << "vk::SystemError: " << err.what() << std::endl;
        exit(-1);
    } catch (...) {
        std::cout << "unknown error\n";
        exit(-1);
    }
    return 0;
}

