#ifndef KATLA_COLUMN_H
#define KATLA_COLUMN_H

#include "katla/core/error.h"
#include "katla/core/subject.h"
#include "katla/core/size.h"

#include "katla/ui-components/widget.h"
#include "katla/ui-components/container.h"

#include <tuple>
#include <memory>
#include <string>
#include <vector>

namespace katla {

    class Column : public Container {
    public:
        Column() = default;
        virtual ~Column() = default;

    };

}

#endif