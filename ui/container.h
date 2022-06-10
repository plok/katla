#ifndef KATLA_CONTAINER_H
#define KATLA_CONTAINER_H

#include "katla/core/error.h"
#include "katla/core/subject.h"
#include "katla/core/size.h"

#include "katla/ui/widget.h"

#include <tuple>
#include <memory>
#include <string>
#include <vector>

namespace katla {

    struct ContainerChild {
        std::shared_ptr<Widget> child;
        bool expand {}; // flex?
        bool fill {};
        int additionSpacing {};

        // TODO
        std::optional<int> width {};
        std::optional<int> height {};
    };

    struct ContainerState {
        enum class Orientation { Horizontal, Vertical };

        Orientation orientation; // TODO double with column / row
        std::vector<ContainerChild> children;

        int spacing {};
    };

    class Container : public Widget  {
    public:
        Container() = default;
        virtual ~Container() = default;

        virtual void updateContainer(const ContainerState& state) = 0;

    };

}

#endif