#include "text-field.h"

#include "ui/platform-register.h"

namespace katla {

std::unique_ptr<TextField> TextField::create() {
    auto reg = PlatformRegister::instance();
    auto builder = reg->activeBuilder();
    return builder->createTextField();
}

}