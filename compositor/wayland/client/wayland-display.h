#ifndef KATLA_WAYLAND_DISPLAY
#define KATLA_WAYLAND_DISPLAY

#include "common/error.h"

#include "wayland-surface.h"
#include "wayland-xdg-shell-surface.h"

#include "compositor/wayland/api/xdg-shell-client-protocol-unstable-v6.h"
#include "wayland-client.h"
#include "wayland-shm-pool.h"

#include <memory>
#include <vector>

class WaylandDisplay
{
public:
    WaylandDisplay();
    virtual ~WaylandDisplay();
    
    ErrorPtr init();

    wl_display* handle() {
        return _display;
    }

    WaylandXdgShellSurfacePtr createSurface();

private:
    static void registryGlobal(void *data, struct wl_registry *registry, uint32_t name, const char *interface, uint32_t version);
    void registryGlobal(struct wl_registry *registry, uint32_t name, const std::string interface, uint32_t version);

    static void registryGlobalRemove(void *data, struct wl_registry *b, uint32_t name);

    WaylandShmPool _pool;

    wl_display* _display;

    wl_registry* _registry;
    wl_registry_listener _registryListener;

    wl_compositor *_compositor;
    wl_pointer *_pointer;
    wl_seat *_seat;
    wl_shell *_shell;
    wl_shm *_shm;
    zxdg_shell_v6 *_xdgShell;

    bool _initialized;
};

#endif