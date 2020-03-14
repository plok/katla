#ifndef GUI_KIT_H
#define GUI_KIT_H

#include "katla-guikit.h"

#include "graphics/graphics-backend.h"
#include "graphics/graphics-configuration.h"

#include "window-management/window.h"

#include "common/error.h"
#include <memory>

class CoreApplication;
class RenderView;

class KATLA_GUIKIT_DECLSPEC GuiKit {
public:
    explicit GuiKit(CoreApplication& coreApp);
    virtual ~GuiKit();

    ErrorPtr init();
    std::tuple<std::shared_ptr<GraphicsBackend>, ErrorPtr> initializeGraphicsBackend(const GraphicsConfiguration& openGlConfiguration);

    ErrorPtr run();
    ErrorPtr close();

    WindowPtr createWindow(std::shared_ptr<WindowProperties> windowProperties, std::shared_ptr<RenderView> renderView);

private:
    CoreApplication* _coreApp;

    std::shared_ptr<GraphicsBackend> _graphicsBackend;

};

#endif