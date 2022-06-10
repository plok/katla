#ifndef KATLA_PLATFORM_REGISTER_H
#define KATLA_PLATFORM_REGISTER_H

#include "katla/core/error.h"
#include "katla/core/subject.h"
#include "katla/core/size.h"

#include "ui/platform-builder.h"

#include <memory>
#include <map>

namespace katla {

class PlatformRegister {
public:
    virtual ~PlatformRegister() = default;

    void registerBuilder(std::string name, std::shared_ptr<PlatformBuilder> builder);
    void activateBuilder(std::string name);

    std::shared_ptr<PlatformBuilder> activeBuilder();

    static std::shared_ptr<PlatformRegister> instance();

private:
    PlatformRegister() = default;

    std::map<std::string, std::shared_ptr<PlatformBuilder>> m_builders;
    std::shared_ptr<PlatformBuilder> m_activeBuilder;

    static std::shared_ptr<PlatformRegister> m_instance;
};

}

#endif
