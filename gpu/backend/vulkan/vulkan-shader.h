#ifndef VULKAN_SHADER_H
#define VULKAN_SHADER_H

#include "common/error.h"

#include "gpu/backend/vulkan/vulkan-device.h"

#include <vulkan/vulkan_core.h>

#include <memory>
#include <vector>
#include <cstdint>

class VulkanFunctionTable;

class VulkanShader;
typedef std::shared_ptr<VulkanShader> VulkanShaderPtr;

class VulkanShader
{
public:
    VulkanShader(std::shared_ptr<VulkanFunctionTable> vft, VulkanDevicePtr vulkanDevice, std::string fileName);
    virtual ~VulkanShader();
    
    ErrorPtr init();

    VkShaderModule vulkanStructure() {
        return m_shaderModule;
    }

private:
    static std::vector<uint8_t> readFile(const std::string& filename);
    std::tuple<VkShaderModule, ErrorPtr> createShaderModule(const std::vector<uint8_t>& code);

    std::shared_ptr<VulkanFunctionTable> m_functionTable;
    VulkanDevicePtr m_vulkanDevice;
    std::string m_fileName;

    VkShaderModule m_shaderModule;

    bool m_initialized;
};

#endif