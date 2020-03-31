#include "wayland-surface.h"

#include <utility>

#include <iostream>
#include <cassert>
#include <algorithm>

WaylandSurface::WaylandSurface(WaylandShmPool& pool) :
    _compositor(nullptr),
    _shell(nullptr),
    _pool(pool),
    _surface(nullptr),
    _shellSurface(nullptr),
    _initialized(false)
{
}

WaylandSurface::~WaylandSurface()
{
    if (_initialized) {
        if (_shellSurface != nullptr) {
            wl_shell_surface_destroy(_shellSurface);
        }
        if (_surface != nullptr) {
            wl_surface_destroy(_surface);
        }
    }
}

ErrorPtr WaylandSurface::init(wl_compositor* compositor, wl_shell* shell)
{
    _compositor = compositor;
    _shell = shell;

    _surface = wl_compositor_create_surface(compositor);
       
    if (_surface == NULL)
        return Error::create("error creating surface!");

    auto buffer = _pool.createBuffer(640, 480);

    // shell surface
    _shellSurface = wl_shell_get_shell_surface(_shell, _surface);

    if (_shellSurface == NULL) {
        wl_surface_destroy(_surface);
        return Error::create("error creating shell surface!");
    }

    _shellSurfaceListener.ping = shellSurfacePing,
    _shellSurfaceListener.configure = shellSurfaceConfigure,

    wl_shell_surface_add_listener(_shellSurface, &_shellSurfaceListener, 0);
    wl_shell_surface_set_toplevel(_shellSurface);
    wl_shell_surface_set_user_data(_shellSurface, this);
    wl_surface_set_user_data(_surface, this);
    
    wl_shell_surface_set_toplevel(_shellSurface);
    wl_shell_surface_set_title(_shellSurface, "hello!");

    // bind buffer
    wl_surface_attach(_surface, buffer->handle(), 10, 10);
    wl_surface_commit(_surface);

    _initialized = true;

    return Error::none();
}

void WaylandSurface::shellSurfacePing(void *data, wl_shell_surface *shell_surface, uint32_t serial)
{
    assert(data != nullptr);

    auto waylandSurface = reinterpret_cast<WaylandSurface*>(data);
    wl_shell_surface_pong(waylandSurface->_shellSurface, serial);
}

void WaylandSurface::shellSurfaceConfigure(void *data, wl_shell_surface *shell_surface, uint32_t edges, int32_t width, int32_t height)
{
    std::cout << "Surface configure: " << edges << width << height << std::endl;
}