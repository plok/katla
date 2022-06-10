#ifndef KATLA_ROW_H
#define KATLA_ROW_H

#include "katla/core/error.h"
#include "katla/core/subject.h"
#include "katla/core/size.h"

#include "katla/ui/widget.h"
#include "katla/ui/container.h"

#include <tuple>
#include <memory>
#include <string>
#include <vector>

namespace katla {

    class Row : public Container {
    public:
        Row() = default;
        virtual ~Row() = default;

    };

}

#endif