#ifndef KATLA_WAYLAND_SHM_POOL
#define KATLA_WAYLAND_SHM_POOL

#include "common/error.h"

#include "compositor/wayland/api/xdg-shell-client-protocol-unstable-v6.h"
#include "wayland-client.h"

#include "wayland-buffer.h"

#include <memory>
#include <vector>

class WaylandShmPool
{
public:
    WaylandShmPool();
    virtual ~WaylandShmPool();
    
    ErrorPtr init(wl_shm* shm);

    WaylandBufferPtr createBuffer(int32_t width, int32_t height);

private:
    wl_shm* _shm;
    wl_shm_pool* _shmPool;

    uint8_t* _buffer;
    size_t _bufferSize;

    int _fd;

    bool _initialized;
};

#endif