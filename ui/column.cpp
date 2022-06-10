#include "column.h"

#include "katla/ui/platform-register.h"

namespace katla {

std::unique_ptr<Column> Column::create() {
    auto reg = PlatformRegister::instance();
    auto builder = reg->activeBuilder();
    return builder->createColumn();
}

}