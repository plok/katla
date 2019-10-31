#ifndef KATLA_DIRECT2D_H
#define KATLA_DIRECT2D_H

#include "direct2d-surface.h"

#include "graphics/graphics-backend.h"
#include "graphics/graphics-configuration.h"

#include "gpu/backend/physical-device.h"
#include "gpu/backend/device.h"

#include "common/error.h"

#include "../../katla-gpu.h"
#include <memory>

class WindowFactory;

struct Direct2D_Private;
struct HWND__;
typedef HWND__* WindowHandle;

class KATLA_GPU_DECLSPEC Direct2D : public GraphicsBackend
{
public:
    Direct2D();
    virtual ~Direct2D();

    ErrorPtr init(const GraphicsConfiguration& configuration);
    void cleanup();

    ErrorPtr initDevice() {
        return Error::none();
    }

    std::unique_ptr<Direct2D_Surface> createSurface(const WindowHandle& handle);

    std::unique_ptr<WindowFactory> windowFactory();
    
private:
    GraphicsConfiguration _graphicsConfiguration;

    std::unique_ptr<Direct2D_Private> _d;
};

#endif
