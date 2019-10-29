#ifndef WINCONTROL_WINDOW_FACTORY_H
#define WINCONTROL_WINDOW_FACTORY_H

#include "katla-window-management-wincontrol.h"

#include "wincontrol-window.h"
#include "graphics/render-view.h"

#include "window-management/window.h"
#include "window-management/window-factory.h"
#include "common/error.h"

#include <memory>
#include <string>

class RenderView;

class KATLA_WINDOW_MANAGEMENT_WINCONTROL_DECLSPEC WinControlWindowFactory : public WindowFactory {
public:
    WinControlWindowFactory();

    std::tuple<WindowPtr, ErrorPtr> create(std::shared_ptr<RenderView> renderView, std::shared_ptr<WindowProperties> properties);
};

#endif