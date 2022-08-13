#ifndef KATLA_UI_COMPONENTS_TEXT_FIELD_H
#define KATLA_UI_COMPONENTS_TEXT_FIELD_H

#include "katla/core/error.h"
#include "katla/core/subject.h"
#include "katla/core/size.h"

#include "katla/ui/widget.h"

#include <tuple>
#include <memory>
#include <string>
#include <vector>

namespace katla {

    struct TextFieldState {
        std::string text;
    };

    class TextField : public Widget {
    public:
        TextField() = default;
        virtual ~TextField() = default;

        static std::unique_ptr<TextFieldState> create();

        virtual void setText(const TextFieldState& state) = 0;

        virtual void show() = 0;

        // virtual std::unique_ptr<Subscription> onClicked(std::function<void(void)> clickedCallback) = 0;
    };

}

#endif