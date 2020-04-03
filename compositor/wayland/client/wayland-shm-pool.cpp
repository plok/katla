#include "wayland-shm-pool.h"

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

#include <sstream>
#include <chrono>
#include <ctime>

WaylandShmPool::WaylandShmPool() :
    _shmPool(nullptr),
    _buffer(nullptr),
    _bufferSize(0),
    _fd(-1),
    _initialized(false)
{
}

WaylandShmPool::~WaylandShmPool()
{
    if (_shmPool) {
        wl_shm_pool_destroy(_shmPool);
    }

    if (_buffer) {
        munmap(_buffer, _bufferSize);
    }

    if (_fd != -1) {
        shm_unlink("/wayland-test-client-1");
    }
}

ErrorPtr WaylandShmPool::init(wl_shm* shm)
{
    _shm = shm;

    int index = std::rand() % 999;

    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);


    std::stringstream sstr;
    sstr << "/wayland-test-client-" << std::ctime(&now_time);

    std::cout << "create shm pool: " << sstr.str() << std::endl;

    // TODO buffer rights!!!
    _fd = shm_open(sstr.str().c_str(), O_CREAT | O_RDWR, S_IRWXO);
    if (_fd == -1) {
        std::cout << strerror(errno) << std::endl;
        return Error::create("Failed opening shared memory");
    }

    _bufferSize = 480 * 640 * 4;
    auto result = ftruncate(_fd, _bufferSize);
    if (result == -1) {
        return Error::create("Failed allocating shared memory");
    }

    _buffer = static_cast<uint8_t*>(mmap(0, _bufferSize, PROT_READ | PROT_WRITE, MAP_SHARED, _fd, 0));
    if (_buffer == MAP_FAILED) {
        return Error::create("Failed mapping shared memory");
    }
    
    _shmPool = wl_shm_create_pool(_shm, _fd, _bufferSize);

    _initialized = true;

    return Error::none();
}

WaylandBufferPtr WaylandShmPool::createBuffer(int32_t width, int32_t height)
{
    auto stride = width * 4 /* TODO channels */;

    wl_buffer *buffer = wl_shm_pool_create_buffer(_shmPool, 0, width, height, stride, WL_SHM_FORMAT_XRGB8888);

    if (buffer == nullptr)
        return WaylandBufferPtr();

    // TODO do not give whole pool
    auto waylandBuffer = std::make_shared<WaylandBuffer>(buffer, _buffer);

    return waylandBuffer;
}