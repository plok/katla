#include "platform-register.h"

namespace katla {

std::shared_ptr<PlatformRegister> PlatformRegister::m_instance;

std::shared_ptr<PlatformRegister> PlatformRegister::instance() {
    if (!m_instance) {
        m_instance = std::shared_ptr<PlatformRegister>(new PlatformRegister());
    }
    return m_instance;
}

void PlatformRegister::registerBuilder(std::string name, std::shared_ptr<PlatformBuilder> builder)
{
    m_builders[name] = builder;
}

void PlatformRegister::activateBuilder(std::string name)
{
    auto findIt = m_builders.find(name);
    if (findIt == m_builders.end()) {
        return; // TODO error;
    }
    m_activeBuilder = findIt->second;
}

std::shared_ptr<PlatformBuilder> PlatformRegister::activeBuilder() {
    assert (m_activeBuilder);

    return m_activeBuilder;
}

}