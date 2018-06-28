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



int main ( int argc, char *argv[] )
{
    try {

        auto instanceExtensionProperties = vk::enumerateInstanceExtensionProperties();

        for ( auto property:  instanceExtensionProperties ) {
            std::cout << "Supported extensions:" << property.extensionName << std::endl ;
        }

        std::vector<const char *> extension_names ( {VK_KHR_DISPLAY_EXTENSION_NAME, VK_KHR_SURFACE_EXTENSION_NAME} );

        auto const appInfo = vk::ApplicationInfo()
                             .setPApplicationName ( AppName )
                             .setApplicationVersion ( 0 )
                             .setPEngineName ( EngineName )
                             .setEngineVersion ( 0 )
                             .setApiVersion ( VK_API_VERSION_1_1 );
        auto const inst_info = vk::InstanceCreateInfo()
                               .setPApplicationInfo ( &appInfo )
                               .setEnabledExtensionCount ( extension_names.size() )
                               .setPpEnabledExtensionNames ( extension_names.data() );


        vk::Instance instance = vk::createInstance ( inst_info, nullptr );
        printf ( "Have instance!\n" );

        auto physicalDevices = instance.enumeratePhysicalDevices();
        printf ( "size: %zu\n", physicalDevices.size() );

        vk::PhysicalDevice gpu = physicalDevices[1];




        printf ( "gpu: %s\n", gpu.getProperties().deviceName );
        /* Look for device extensions */
        std::vector<const char *> device_extension_names ( {VK_KHR_EXTERNAL_MEMORY_EXTENSION_NAME, VK_KHR_SWAPCHAIN_EXTENSION_NAME} );
        auto extensionProperties = gpu.enumerateDeviceExtensionProperties();
        for ( auto i: extensionProperties ) {
            printf ( "Supperted device extensions: %s\n",i.extensionName );
        }


        /* Layers */
        std::vector<const char *> enabledLayers ( {"VK_LAYER_LUNARG_standard_validation"} );

        auto layerProperties = vk::enumerateInstanceLayerProperties();
        for ( auto layerProperty : layerProperties ) {
            printf ( "layer property supported: %s \n", layerProperty.layerName );
        }


        auto queueFamilyProperties = gpu.getQueueFamilyProperties();

        uint32_t graphicsQueueFamilyIndex = 0;
        for ( size_t i = 0; i < queueFamilyProperties.size(); i++ ) {
            if ( queueFamilyProperties[i].queueFlags & vk::QueueFlagBits::eGraphics ) {
                graphicsQueueFamilyIndex = i;
                break;
            }
        }

        printf ( "Selected graphicsQueue, %d of %zu \n", graphicsQueueFamilyIndex,  queueFamilyProperties.size() );
        float priorities[] = { 0.0f };
        auto queueCreateInfo = vk::DeviceQueueCreateInfo()
                               .setPNext ( NULL )
                               .setQueueCount ( 1 )
                               .setQueueFamilyIndex ( graphicsQueueFamilyIndex )
                               .setPQueuePriorities ( priorities );

        std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos ( {queueCreateInfo});
        auto deviceInfo = vk::DeviceCreateInfo()
                          .setPNext ( NULL )
                          .setEnabledLayerCount ( enabledLayers.size() )
                          .setPpEnabledLayerNames ( enabledLayers.data() )
                          .setPQueueCreateInfos ( &queueCreateInfo )
                          .setEnabledExtensionCount ( device_extension_names.size() )
                          .setPpEnabledExtensionNames ( device_extension_names.data() );
        printf ( "Creating device \n" );


        vk::Device device = gpu.createDevice ( deviceInfo );

        auto displayPropertiesList = gpu.getDisplayPropertiesKHR();

        assert(displayPropertiesList.size() > 0 );

        auto propertiesKHR = displayPropertiesList.front();
        const auto display = propertiesKHR.display;

        auto displayModePropertiesKHR = gpu.getDisplayModePropertiesKHR(display);

        auto displayPlanePropertiesKHR = gpu.getDisplayPlanePropertiesKHR();

        vk::DisplayPlanePropertiesKHR selectedPlane;
        for (auto plane : displayPlanePropertiesKHR) {
            if (plane.currentDisplay && plane.currentDisplay != display) {
                continue;
            }
            auto supportedDisplaysKHR = gpu.getDisplayPlaneSupportedDisplaysKHR(plane.currentStackIndex);
            if (supportedDisplaysKHR.size() > 0) {
                selectedPlane = plane;
                printf("yoohoooo\n" );
            }
            
        }



        auto const createInfo = vk::DisplaySurfaceCreateInfoKHR()
                .setDisplayMode(displayModePropertiesKHR.front().displayMode)
                .setPlaneIndex(1)
                .setPlaneStackIndex(selectedPlane.currentStackIndex)
                .setGlobalAlpha(1.0f)
                ;
        auto surfaceKHR = instance.createDisplayPlaneSurfaceKHR(createInfo);
        if(surfaceKHR) {
            printf("I have many successes");
        }

//
//        instance.createDisplayPlaneSurfaceKHR(&createInfo, nullptr, &surface);

        /* VULKAN_HPP_KEY_END */
    } catch ( vk::SystemError err ) {
        std::cout << "vk::SystemError: " << err.what() << std::endl;
        exit ( -1 );
    } catch ( ... ) {
        std::cout << "unknown error\n";
        exit ( -1 );
    }
    return 0;
}

