#ifndef KATLA_WIDGET_H
#define KATLA_WIDGET_H

#include "katla/core/error.h"
#include "katla/core/subject.h"
#include "katla/core/size.h"

#include <tuple>
#include <memory>
#include <string>

namespace katla {

    class Widget;

    typedef std::shared_ptr<Widget> ContainerPtr;

    class Widget : public std::enable_shared_from_this<Widget> {
    public:
        Widget() = default;
        virtual ~Widget() = default;

        // TODO GUID?
        //virtual int id() = 0;

    };

}

#endif