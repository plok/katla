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

        auto instanceExtensionProperties = vk::enumerateInstanceExtensionProperties();


        for (auto property:  instanceExtensionProperties) {
            std::cout << "Supported extensions:" << property.extensionName << std::endl ;
        }


        std::vector<const char *> extension_names({VK_KHR_DISPLAY_EXTENSION_NAME});


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


        vk::Instance instance = vk::createInstance(inst_info, nullptr);

        auto physicalDevices = instance.enumeratePhysicalDevices();

        vk::PhysicalDevice gpu = physicalDevices[1];


        /* Look for device extensions */
        std::vector<const char *> device_extension_names({VK_KHR_EXTERNAL_MEMORY_EXTENSION_NAME});
        auto extensionProperties = gpu.enumerateDeviceExtensionProperties();


        auto deviceInfo = vk::DeviceCreateInfo()
                .setEnabledExtensionCount(device_extension_names.size())
                .setPpEnabledExtensionNames(device_extension_names.data());

        vk::Device device = gpu.createDevice(deviceInfo, nullptr);
        printf("Have device!\n");
        fflush(stdout);
        // Get the first display

        auto displayPropertiesKHR = gpu.getDisplayPropertiesKHR();


        if (displayPropertiesKHR.size() == 0) {
            printf("Cannot find any displaysdas!\n");
            fflush(stdout);
            exit(1);
        }

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


        display = display_props.display;

        std::cout << "display:" << display_props.displayName << std::endl;
        auto result = gpu.getDisplayModePropertiesKHR(display, &mode_count, nullptr);

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
