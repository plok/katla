#ifndef OPENGL_WINDOW_FACTORY_H
#define OPENGL_WINDOW_FACTORY_H

#include "gpu/backend/opengl/opengl-window.h"

#include "window/window.h"
#include "window/window-factory.h"
#include "common/error.h"

#include <memory>
#include <string>

class RenderView;

class OpenGlWindowFactory : public WindowFactory {
public:
    OpenGlWindowFactory();

    std::tuple<WindowPtr, ErrorPtr> create(std::shared_ptr<RenderView> renderView, std::shared_ptr<WindowProperties> properties);
private:

};

#endif