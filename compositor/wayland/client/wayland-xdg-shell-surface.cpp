#include "wayland-xdg-shell-surface.h"

#include "wayland-display.h"

#include <utility>

#include <iostream>
#include <cassert>
#include <algorithm>

WaylandXdgShellSurface::WaylandXdgShellSurface(WaylandDisplay& display, WaylandShmPool& pool) :
    _compositor(nullptr),
    _shell(nullptr),
    _display(display),
    _pool(pool),
    _surface(nullptr),
    _shellSurface(nullptr),
    _topLevel(nullptr),
    _initialized(false)
{
}

WaylandXdgShellSurface::~WaylandXdgShellSurface()
{
    if (_initialized) {
        if (_topLevel != nullptr) {
            zxdg_toplevel_v6_destroy(_topLevel);
        }
        if (_shellSurface != nullptr) {
            zxdg_surface_v6_destroy(_shellSurface);
        }
        if (_surface != nullptr) {
            wl_surface_destroy(_surface);
        }
    }
}

ErrorPtr WaylandXdgShellSurface::init(wl_compositor* compositor, zxdg_shell_v6* shell)
{
    _compositor = compositor;
    _shell = shell;

    _surface = wl_compositor_create_surface(compositor);
       
    if (_surface == NULL)
        return Error::create("error creating surface!");

    wl_surface_add_listener(_surface, &_surfaceListener, this);

    // TODO: size
    _buffer = _pool.createBuffer(640, 480);

    // shell surface
    _shellSurface = zxdg_shell_v6_get_xdg_surface(_shell, _surface);

    if (_shellSurface == NULL) {
        wl_surface_destroy(_surface);
        return Error::create("error creating shell surface!");
    }

    _shellListener.ping = shellPing;
    zxdg_shell_v6_add_listener(_shell, &_shellListener, this);

    _shellSurfaceListener.configure = shellSurfaceConfigure;

    zxdg_surface_v6_add_listener(_shellSurface, &_shellSurfaceListener, this);

    zxdg_surface_v6_set_user_data(_shellSurface, this);
    wl_surface_set_user_data(_surface, this);
    
    // TODO handle null return
    _topLevel = zxdg_surface_v6_get_toplevel(_shellSurface);

    _topLevelListener.configure = topLevelConfigure;
    _topLevelListener.close = topLevelClose;

    zxdg_toplevel_v6_add_listener(_topLevel, &_topLevelListener, this);

    zxdg_toplevel_v6_set_title(_topLevel, "hello!");
    zxdg_toplevel_v6_set_app_id(_topLevel, "wayland-test-client");

    // bind buffer
    wl_surface_commit(_surface);
	wl_display_roundtrip(_display.handle());

    wl_surface_attach(_surface, _buffer->handle(), 10, 10);
	wl_surface_commit(_surface);

    _initialized = true;

    return Error::none();
}

void WaylandXdgShellSurface::surfaceEnter(void *data,
            struct wl_surface *wl_surface,
            struct wl_output *output)
{
std::cout << "Surface enter" << std::endl;
}

void WaylandXdgShellSurface::surfaceLeave(void *data,
            struct wl_surface *wl_surface,
            struct wl_output *output)
{
std::cout << "Surface leave" << std::endl;
}

void WaylandXdgShellSurface::shellPing(void *data, zxdg_shell_v6 *shell, uint32_t serial)
{
    assert(data != nullptr);

    zxdg_shell_v6_pong(shell, serial);
}

void WaylandXdgShellSurface::shellSurfaceConfigure(void *data, zxdg_surface_v6 *shellSurface, uint32_t serial)
{
    std::cout << "Surface configure" << std::endl;

    auto waylandSurface = reinterpret_cast<WaylandXdgShellSurface*>(data);
    zxdg_surface_v6_ack_configure(waylandSurface->_shellSurface, serial);
}

void WaylandXdgShellSurface::topLevelConfigure(void *data,
            struct zxdg_toplevel_v6 *zxdg_toplevel_v6,
            int32_t width,
            int32_t height,
            struct wl_array *states)
{
    std::cout << "TopLevel configure: " << width << height << std::endl;
}

void WaylandXdgShellSurface::topLevelClose(void *data,
            struct zxdg_toplevel_v6 *zxdg_toplevel_v6)
{
    std::cout << "TopLevel close" << std::endl;
}
