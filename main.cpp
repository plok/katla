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

static char const* AppName = "02_EnumerateDevices";
static char const* EngineName = "Vulkan.hpp";

int main(int argc, char *argv[])
{
    try
    {

        uint32_t instance_extension_count = 0;
        auto result = vk::enumerateInstanceExtensionProperties(nullptr, &instance_extension_count, nullptr);
        assert (result == vk::Result::eSuccess);

        std::unique_ptr<vk::ExtensionProperties[]> instance_extensions(new vk::ExtensionProperties[instance_extension_count]);
        result = vk::enumerateInstanceExtensionProperties(nullptr, &instance_extension_count, instance_extensions.get());
        assert (result == vk::Result::eSuccess);

        for (uint32_t i = 0; i < instance_extension_count; i++) {
            std::cout << "Supported extensions:" << instance_extensions[i].extensionName << std::endl ;
        }



        uint32_t enabled_extension_count = 0;
        char const *extension_names[64];
        memset(extension_names, 0, sizeof(extension_names));
        extension_names[enabled_extension_count++] =
                VK_KHR_DISPLAY_EXTENSION_NAME;


        auto const appInfo = vk::ApplicationInfo()
                .setPApplicationName(AppName)
                .setApplicationVersion(0)
                .setPEngineName(EngineName)
                .setEngineVersion(0)
                .setApiVersion(VK_API_VERSION_1_1);
        auto const inst_info = vk::InstanceCreateInfo()
                .setPApplicationInfo(&appInfo)
                .setEnabledExtensionCount(enabled_extension_count)
                .setPpEnabledExtensionNames(extension_names);


        vk::Instance instance;
        result = vk::createInstance(&inst_info, nullptr, &instance);
        assert(result == vk::Result::eSuccess);

        uint32_t gpu_count = 0;
        vk::PhysicalDevice gpu;
        result = instance.enumeratePhysicalDevices(&gpu_count, nullptr);
        assert(result == vk::Result::eSuccess);
        std::cout << "Checking: " << "\n" ;

        std::cout << "gpu_count: " << gpu_count <<"\n" ;

        if (gpu_count > 0) {
            std::unique_ptr<vk::PhysicalDevice[]> physical_devices(new vk::PhysicalDevice[gpu_count]);
            result = instance.enumeratePhysicalDevices(&gpu_count, physical_devices.get());
            assert(result == vk::Result::eSuccess);

            std::cout << "" << physical_devices[0].getProperties().deviceName << std::endl;
            gpu = physical_devices[0];
        } else {
            assert(false);
        }

        auto deviceInfo = vk::DeviceCreateInfo()
                .setEnabledExtensionCount(enabled_extension_count)
                .setPpEnabledExtensionNames(extension_names);

        vk::Device device;
        result = gpu.createDevice(&deviceInfo, nullptr, &device);
        assert((result == vk::Result::eSuccess));
        printf("Have device!\n");


        /* Look for device extensions */
        uint32_t device_extension_count = 0;
        vk::Bool32 swapchainExtFound = VK_FALSE;
        enabled_extension_count = 0;
        memset(extension_names, 0, sizeof(extension_names));

        result = gpu.enumerateDeviceExtensionProperties(nullptr, &device_extension_count, nullptr);
        assert(result == vk::Result::eSuccess);

        if (device_extension_count > 0) {
            std::unique_ptr<vk::ExtensionProperties[]> device_extensions(new vk::ExtensionProperties[device_extension_count]);
            result = gpu.enumerateDeviceExtensionProperties(nullptr, &device_extension_count, device_extensions.get());
            assert(result == vk::Result::eSuccess);

            for (uint32_t i = 0; i < device_extension_count; i++) {
                std::cout << device_extensions[i].extensionName << std::endl;
                if (!strcmp(VK_KHR_SWAPCHAIN_EXTENSION_NAME, device_extensions[i].extensionName)) {
                    swapchainExtFound = VK_TRUE;
                    std::cout << "found swapchain" << std::endl;
                    extension_names[enabled_extension_count++] = VK_KHR_SWAPCHAIN_EXTENSION_NAME;
                } else if (!strcmp(VK_KHR_EXTERNAL_MEMORY_EXTENSION_NAME, device_extensions[i].extensionName)) {
                    std::cout << "found ext_memory" << std::endl;

                    extension_names[enabled_extension_count++] = VK_KHR_EXTERNAL_MEMORY_EXTENSION_NAME;
                }
                assert(enabled_extension_count < 64);
            }
        }
        assert(swapchainExtFound);

        uint32_t display_count = 0;

        // Get the first display
        std::cout << "sdawqeqwdasdas" << std::endl;

        result = gpu.getDisplayPropertiesKHR(&display_count, nullptr);
        assert((result == vk::Result::eSuccess) ||
               (result == vk::Result::eIncomplete));

        std::cout << "sdadasdas" << std::endl;

        if (display_count == 0) {
            printf("Cannot find any display!\n");
            fflush(stdout);
            exit(1);
        }

        display_count = 1;
        printf("Have display!\n");


        uint32_t mode_count;
        uint32_t plane_count =0;

        vk::DisplayPropertiesKHR display_props;
        vk::DisplayKHR display;
        vk::DisplayModePropertiesKHR mode_props;
        vk::DisplayPlanePropertiesKHR *plane_props;
        vk::Bool32 found_plane = VK_FALSE;
        uint32_t plane_index;
        vk::Extent2D image_extent;

        vk::SurfaceKHR surface;
        result = gpu.getDisplayPropertiesKHR(&display_count, &display_props);
        assert((result == vk::Result::eSuccess) ||
               (result == vk::Result::eIncomplete));

        display = display_props.display;

        std::cout << "display:" << display_props.displayName << std::endl;
        result = gpu.getDisplayModePropertiesKHR(display, &mode_count, nullptr);

        if (mode_count == 0) {
            printf("Cannot find any mode for the display!\n");
            fflush(stdout);
            exit(1);
        }
        mode_count =1 ;
        printf("Have mode!\n");


        result = gpu.getDisplayModePropertiesKHR(display, &mode_count, &mode_props);
        assert((result == vk::Result::eSuccess) ||
               (result == vk::Result::eIncomplete));
        printf("Have modeProperties!\n");

        result = gpu.getDisplayPlanePropertiesKHR(&plane_count, nullptr);
        assert(result == vk::Result::eSuccess);
        if (plane_count == 0) {
            printf("Cannot find any plane!\n");
            fflush(stdout);
            exit(1);
        }

        std::cout << "Plane_count: " << plane_count << std::endl;
        plane_props = (vk::DisplayPlanePropertiesKHR *)
                malloc(sizeof(vk::DisplayPlanePropertiesKHR) * plane_count);
        assert(plane_props != nullptr);

        result = gpu.getDisplayPlanePropertiesKHR(&plane_count, plane_props);
        assert(result == vk::Result::eSuccess);
        printf("Have planeCount!\n");


        // Find a plane compatible with the display
        for (plane_index = 0; plane_index < plane_count; plane_index++) {
            uint32_t supported_count;
            vk::DisplayKHR *supported_displays;
            printf("Checking plane!\n");

            std::cout << "current display: " << plane_props[plane_index].currentDisplay <<"\n" ;
            std::cout << "display: " << display <<"\n" ;

//            // Disqualify planes that are bound to a different display
            if (plane_props[plane_index].currentDisplay &&
                (plane_props[plane_index].currentDisplay != display)) {
                continue;
            }

            printf("Checking display!\n");

            result = gpu.getDisplayPlaneSupportedDisplaysKHR(plane_index,
                                                             &supported_count,
                                                             nullptr);

            printf("Found compatible plane!\n");


            if (supported_count == 0) {
                continue;
            }

            supported_displays = (vk::DisplayKHR *)
                    malloc(sizeof(vk::DisplayKHR) * supported_count);

            result = gpu.getDisplayPlaneSupportedDisplaysKHR(plane_index,
                                                             &supported_count,
                                                             supported_displays);


            for (uint32_t i = 0; i < supported_count; i++) {
                if (supported_displays[i] == display) {
                    found_plane = VK_TRUE;
                    break;
                }
            }

            free(supported_displays);

            if (found_plane) {
                break;
            }
        }

        std::cout << "found_plane: " << found_plane <<"\n" ;

        if (!found_plane) {
            printf("Cannot find a plane compatible with the display!\n");
            fflush(stdout);
            exit(1);
        }

        free(plane_props);



        auto const createInfo = vk::DisplaySurfaceCreateInfoKHR()
                .setDisplayMode(mode_props.displayMode)
                .setPlaneIndex(plane_index)
                .setPlaneStackIndex(plane_props[plane_index].currentStackIndex)
                .setGlobalAlpha(1.0f)
                .setImageExtent(image_extent);

        instance.createDisplayPlaneSurfaceKHR(&createInfo, nullptr, &surface);

        /* VULKAN_HPP_KEY_END */
    }
    catch (vk::SystemError err)
    {
        std::cout << "vk::SystemError: " << err.what() << std::endl;
        exit(-1);
    }
    catch (...)
    {
        std::cout << "unknown error\n";
        exit(-1);
    }
    return 0;
}

void init_vk_swapchain() {

}
