#ifndef KATLA_WAYLAND_XDG_SHELL_SURFACE
#define KATLA_WAYLAND_XDG_SHELL_SURFACE

#include "common/error.h"

#include "wayland-client.h"

#include "wayland-shm-pool.h"

#include <memory>
#include <vector>

class WaylandXdgShellSurface;
typedef std::shared_ptr<WaylandXdgShellSurface> WaylandXdgShellSurfacePtr;

class WaylandDisplay;

class WaylandXdgShellSurface
{
public:
    WaylandXdgShellSurface(WaylandDisplay& display, WaylandShmPool& pool);
    virtual ~WaylandXdgShellSurface();
    
    ErrorPtr init(wl_compositor* compositor, zxdg_shell_v6* shell);

    wl_surface* handle() {
        return _surface;
    }

    WaylandBufferPtr buffer() {
        return _buffer;
    }

private:
    static void surfaceEnter(void *data,
		      struct wl_surface *wl_surface,
		      struct wl_output *output);
	
	static void surfaceLeave(void *data,
		      struct wl_surface *wl_surface,
		      struct wl_output *output);

    static void shellPing(void *data, zxdg_shell_v6 *shell, uint32_t serial);
    static void shellSurfaceConfigure(void *data, zxdg_surface_v6 *shell_surface, uint32_t serial);
    
    static void topLevelConfigure(void *data,
			  struct zxdg_toplevel_v6 *zxdg_toplevel_v6,
			  int32_t width,
			  int32_t height,
			  struct wl_array *states);

    static void topLevelClose(void *data,
		      struct zxdg_toplevel_v6 *zxdg_toplevel_v6);

    WaylandDisplay& _display;
    WaylandBufferPtr _buffer;

    wl_compositor* _compositor;
    zxdg_shell_v6* _shell;
    WaylandShmPool& _pool;

    wl_surface* _surface;
    zxdg_surface_v6* _shellSurface;
    zxdg_toplevel_v6* _topLevel;

    wl_surface_listener _surfaceListener;
    zxdg_shell_v6_listener _shellListener;
    zxdg_surface_v6_listener _shellSurfaceListener;

    // TODO move to toplevel impl
    zxdg_toplevel_v6_listener _topLevelListener;

    bool _initialized;
};

#endif