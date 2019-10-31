#include "direct2d.h"
#include "direct2d-private.h"

#include "window-management/window-factory.h"
#include "gpu/backend/opengl/opengl-window-factory.h"

#include "window-management-wincontrol/wincontrol-window.h"

#include <cstring>
#include <iostream>
#include <vector>

typedef HWND WindowHandle;

Direct2D::Direct2D()
{
    _d = std::make_unique<Direct2D_Private>();
}

Direct2D::~Direct2D()
{
}

ErrorPtr Direct2D::init(const GraphicsConfiguration& graphicsConfiguration)
{
    _graphicsConfiguration = graphicsConfiguration;

     HRESULT hr = D2D1CreateFactory(
         D2D1_FACTORY_TYPE_SINGLE_THREADED,
         &_d->factory);

    std::cout << "d2d: " << hr << std::endl;

    // if (!glfwInit()) {
    //     return Error::create("Failed initializing GLFW!");
    // }

    // glfwSetErrorCallback(glfw_opengl_error_callback);

    return Error::none();
}

void Direct2D::cleanup()
{
    // TODO out of destructor?
    // glfwTerminate();
}

std::unique_ptr<Direct2D_Surface> Direct2D::createSurface(const WindowHandle& handle)
{
    Direct2D_SurfacePrivateDeps deps {handle, _d->factory};

    return Direct2D_Surface::create(deps);
}

std::unique_ptr<WindowFactory> Direct2D::windowFactory()
{
    // return std::make_unique<Direct2DWindowFactory>(_openGlConfiguration);
    return std::unique_ptr<WindowFactory>();
}
