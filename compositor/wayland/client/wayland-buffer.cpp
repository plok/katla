#include "wayland-buffer.h"

#include <utility>

#include <iostream>
#include <cassert>
#include <algorithm>

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

WaylandBuffer::WaylandBuffer(wl_buffer* buffer, uint8_t* data) :
    _buffer(buffer),
    _data(data),
    _initialized(false)
{
    std::cout << "buffer created!" << std::endl;
}

WaylandBuffer::~WaylandBuffer()
{
    if (_buffer) {
        wl_buffer_destroy(_buffer);

        std::cout << "buffer destroyed!" << std::endl;
    }
}

ErrorPtr WaylandBuffer::init()
{
    _initialized = true;

    // TODO buffer listener???

    return Error::none();
}
