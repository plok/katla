#ifndef KATLA_UI_COMPONENTS_TEXT_H
#define KATLA_UI_COMPONENTS_TEXT_H

#include "katla/core/error.h"
#include "katla/core/subject.h"
#include "katla/core/size.h"

#include "katla/ui-components/widget.h"

#include <tuple>
#include <memory>
#include <string>
#include <vector>

namespace katla {

    struct TextState {
        std::string text;
    };

    class Text : public Widget {
    public:
        Text() = default;
        virtual ~Text() = default;

        virtual void updateText(const TextState& state) = 0;

        // virtual std::unique_ptr<Subscription> onClicked(std::function<void(void)> clickedCallback) = 0;
    };

}

#endif