#ifndef KATLA_BUTTON_H
#define KATLA_BUTTON_H

#include "katla/core/error.h"
#include "katla/core/subject.h"
#include "katla/core/size.h"

#include "katla/ui/widget.h"

#include <tuple>
#include <memory>
#include <string>
#include <vector>

namespace katla {

    struct ButtonState {
        std::string label;
    };

    class Button : public Widget {
    public:
        Button() = default;
        virtual ~Button() = default;

        static std::unique_ptr<Button> create();

        virtual void updateButton(const ButtonState& state) = 0;

        virtual std::unique_ptr<Subscription> onClicked(std::function<void(void)> clickedCallback) = 0;
    };

}

#endif