#ifndef KATLA_WAYLAND_SURFACE
#define KATLA_WAYLAND_SURFACE

#include "common/error.h"

#include "wayland-client.h"

#include "wayland-shm-pool.h"

#include <memory>
#include <vector>

class WaylandSurface;
typedef std::shared_ptr<WaylandSurface> WaylandSurfacePtr;

class WaylandSurface
{
public:
    WaylandSurface(WaylandShmPool& pool);
    virtual ~WaylandSurface();
    
    ErrorPtr init(wl_compositor* compositor, wl_shell* shell);

    wl_surface* handle() {
        return _surface;
    }

private:
    static void shellSurfacePing(void *data, wl_shell_surface *shell_surface, uint32_t serial);
    static void shellSurfaceConfigure(void *data, wl_shell_surface *shell_surface, uint32_t edges, int32_t width, int32_t height);
    
    wl_compositor* _compositor;
    wl_shell* _shell;
    WaylandShmPool& _pool;

    wl_surface* _surface;
    wl_shell_surface* _shellSurface;

    wl_shell_surface_listener _shellSurfaceListener;

    bool _initialized;
};

#endif