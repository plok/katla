#ifndef GUI_APPLICATION_H
#define GUI_APPLICATION_H

#include "gpu/backend/graphics-backend.h"
#include "gpu/window/window.h"

#include "common/error.h"
#include <memory>

class CoreApplication;
class RenderView;

class GuiKit {
public:
    explicit GuiKit(CoreApplication& coreApp);
    virtual ~GuiKit();

    ErrorPtr init();
    std::tuple<std::shared_ptr<GraphicsBackend>, ErrorPtr> initializeGraphicsBackend(bool useOpenGL);

    ErrorPtr run();
    ErrorPtr close();

    WindowPtr createWindow(std::shared_ptr<WindowProperties> windowProperties, std::shared_ptr<RenderView> renderView);

private:
    CoreApplication* _coreApp;

    std::shared_ptr<GraphicsBackend> _graphicsBackend;

};

#endif