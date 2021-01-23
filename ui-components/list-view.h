#ifndef KATLA_LIST_VIEW_H
#define KATLA_LIST_VIEW_H

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

    class ListView : public Container {
    public:
        ListView() = default;
        virtual ~ListView() = default;

    };

}

#endif