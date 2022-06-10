#include "text.h"

#include "ui/platform-register.h"

namespace katla {

std::unique_ptr<Text> Text::create() {
    auto reg = PlatformRegister::instance();
    auto builder = reg->activeBuilder();
    return builder->createText();
}

}