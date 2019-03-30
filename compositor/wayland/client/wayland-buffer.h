#ifndef KATLA_WAYLAND_BUFFER
#define KATLA_WAYLAND_BUFFER

#include "common/error.h"

#include "compositor/wayland/api/xdg-shell-client-protocol-unstable-v6.h"
#include "wayland-client.h"

#include "wayland-buffer.h"

#include <memory>
#include <vector>

class WaylandBuffer;
typedef std::shared_ptr<WaylandBuffer> WaylandBufferPtr;

class WaylandBuffer
{
public:
    WaylandBuffer(wl_buffer* buffer, uint8_t* data);
    virtual ~WaylandBuffer();
    
    ErrorPtr init();

    wl_buffer* handle() {
        return _buffer;
    }

    uint8_t* data() {
        return _data;
    }

private:
    wl_buffer* _buffer;
    uint8_t* _data;

    bool _initialized;
};

#endif