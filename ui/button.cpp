#include "button.h"

#include "ui/platform-register.h"

namespace katla {

std::unique_ptr<Button> Button::create() {
    auto reg = PlatformRegister::instance();
    auto builder = reg->activeBuilder();
    return builder->createButton();
}

}