#include "vulkan-shader.h"

#include <utility>

#include "vulkan.h"
#include "vulkan-function-table.h"

#include <string>
#include <fstream>
#include <sstream>

VulkanShader::VulkanShader(
        std::shared_ptr<VulkanFunctionTable> vft,
        VulkanDevicePtr vulkanDevice,
        std::string fileName) :
    m_functionTable(std::move(vft)),
    m_vulkanDevice(vulkanDevice),
    m_fileName(fileName),
    m_initialized(false)
{
}

VulkanShader::~VulkanShader()
{
    // TODO look at vulkan handle?
    if (m_initialized) {
        m_functionTable->DestroyShaderModule(m_vulkanDevice->vulkanHandle(), m_shaderModule, nullptr);
    }
}

ErrorPtr VulkanShader::init()
{
    auto shaderCode = readFile(m_fileName);

    auto [shaderModule, error] = createShaderModule(shaderCode);
    if (error) {
        return error;
    }
    
    m_shaderModule = shaderModule;
    m_initialized = true;

    return Error::none();
}

std::vector<uint8_t> VulkanShader::readFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        std::stringstream message;
        message << "failed to open shader file: " << filename;
        throw std::runtime_error(message.str());
    }

    size_t fileSize = (size_t) file.tellg();
    std::vector<uint8_t> buffer(fileSize);

    file.seekg(0);
    file.read(reinterpret_cast<char*>(buffer.data()), fileSize);

    file.close();

    return buffer;
}

std::tuple<VkShaderModule, ErrorPtr> VulkanShader::createShaderModule(const std::vector<uint8_t>& code)
{
    VkShaderModuleCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    VkShaderModule shaderModule;
    if (m_functionTable->CreateShaderModule(m_vulkanDevice->vulkanHandle(), &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
        return {shaderModule, Error::create("failed to create shader module!")};
    }

    std::cout << "shader " << m_fileName << " created!" << std::endl;

    return {shaderModule, Error::none()};
}