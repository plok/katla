#ifndef KATLA_WINDOW_FACTORY_H
#define KATLA_WINDOW_FACTORY_H

#include "katla-window-management.h"

#include "window.h"

#include "katla/core/error.h"

#include <tuple>

namespace katla {

class KATLA_WINDOW_MANAGEMENT_DECLSPEC WindowFactory {
public:
    WindowFactory();
    virtual ~WindowFactory() = default;

    virtual outcome::result<WindowPtr, Error> create() = 0;
};

}

#endif