#ifndef OPENGL_WINDOW_FACTORY_H
#define OPENGL_WINDOW_FACTORY_H

#include "gpu/opengl/opengl-window.h"

#include "window/window.h"
#include "window/window-factory.h"
#include "common/error.h"

#include <memory>
#include <string>

class OpenGlWindowFactory : public WindowFactory {
public:
    OpenGlWindowFactory();

    std::tuple<WindowPtr, ErrorPtr> create(int x, int y, std::string title, std::shared_ptr<WindowEvents> events);
private:

};

#endif