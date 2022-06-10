#include "application.h"

#include "ui/platform-register.h"

namespace katla {

std::unique_ptr<Application> Application::create(int argc, char* argv[], std::string appName)
{
    auto reg = PlatformRegister::instance();
    auto builder = reg->activeBuilder();
    return builder->createApplication(argc, argv, appName);
}

}