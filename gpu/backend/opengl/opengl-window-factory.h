#ifndef OPENGL_WINDOW_FACTORY_H
#define OPENGL_WINDOW_FACTORY_H

#include "gpu/backend/opengl/opengl-window.h"

#include "graphics/render-view.h"
#include "graphics/graphics-configuration.h"

#include "opengl-renderer-binders.h"

#include "window-management/window.h"
#include "window-management/window-factory.h"
#include "common/error.h"

#include <memory>
#include <string>

class RenderView;

class OpenGlWindowFactory : public WindowFactory {
public:
    OpenGlWindowFactory(const GraphicsConfiguration& configuration);

    std::tuple<WindowPtr, ErrorPtr> create(std::shared_ptr<RenderView> renderView, std::shared_ptr<WindowProperties> properties);
private:
    GraphicsConfiguration _configuration;
    OpenGlRendererBinders _binders;

};

#endif