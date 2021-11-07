#ifndef KATLA_WINDOW_H
#define KATLA_WINDOW_H

#include "katla-window-management.h"

#include "katla/core/error.h"
#include "katla/core/subject.h"
#include "katla/core/size.h"

#include "katla/ui-components/container.h"

#include <tuple>
#include <memory>
#include <string>

namespace katla {

    class Window;

    typedef std::shared_ptr<Window> WindowPtr;

    struct WindowProperties {
        Size_32s size;
        std::string title;

        // TODO remove
        // std::shared_ptr<Observer<Size_32s>> sizeChanged;
        // std::shared_ptr<Observer<bool>> closeRequested;
    };

    class KATLA_WINDOW_MANAGEMENT_DECLSPEC Window : public Container {
    public:
        Window();

        virtual ~Window() = default;

        virtual void init() = 0;
        virtual void close() = 0;

        virtual void show() = 0;

        virtual void processEvents() = 0;

    };

}

#endif