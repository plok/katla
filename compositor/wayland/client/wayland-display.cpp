#include "wayland-display.h"

#include <utility>

#include <iostream>
#include <cassert>
#include <algorithm>

WaylandDisplay::WaylandDisplay() :
    _display(nullptr),
    _registry(nullptr),
    _compositor(nullptr),
    _pointer(nullptr),
    _seat(nullptr),
    _shell(nullptr),
    _shm(nullptr),
    _initialized(false)
{
}

WaylandDisplay::~WaylandDisplay()
{
    //wl_pointer_destroy(pointer);

    if (_seat != nullptr) {
        wl_seat_destroy(_seat);
    }
    if (_shell != nullptr) {
        wl_shell_destroy(_shell);
    }
    if (_shm != nullptr) {
        wl_shm_destroy(_shm);
    }
    if (_compositor != nullptr) {
        wl_compositor_destroy(_compositor);
    }

    if (_registry != nullptr) {
        wl_registry_destroy(_registry);
        _registry = nullptr;
    }

    if (_display != nullptr) {
        wl_display_disconnect(_display);
        _display = nullptr;
        std::cout << "wl-display disconnected" << std::endl;
    }

}

ErrorPtr WaylandDisplay::init()
{
    std::cout << "starting wl-display init" << std::endl;

    _display = wl_display_connect(nullptr);
	if (_display == nullptr) {
		return Error::create("failed to create display connection");
    }

    _registryListener.global = registryGlobal;
    _registryListener.global_remove = registryGlobalRemove;

    _registry = wl_display_get_registry(_display);
    wl_registry_add_listener(_registry, &_registryListener, this);

    wl_display_roundtrip(_display);
    
    if (_shm == nullptr) {
        std::cerr << "Could not find wl_shm interface" << std::endl;
        return Error::create("Could not find wl_shm interface");
    };
    
    auto poolError = _pool.init(_shm);
    if (poolError) {
        std::cerr << poolError->message << std::endl;
        return poolError;
    }

    std::cout << "Pool created!" << std::endl;

    _initialized = true;

    std::cout << "wl-display created!!" << std::endl;

    return Error::none();
}

WaylandXdgShellSurfacePtr WaylandDisplay::createSurface()
{
    if (_compositor == nullptr || _shell == nullptr) {
        // TODO exceptions??
        throw std::runtime_error("Cannot create surface when compositor or shell is not initialized!");
    }

    auto surface = std::make_shared<WaylandXdgShellSurface>(*this, _pool);
    surface->init(_compositor, _xdgShell);

    std::cout << "surface created!" << std::endl;

    return surface;
}

void WaylandDisplay::registryGlobal(void *data, struct wl_registry *registry, uint32_t name, const char *interface_cstr, uint32_t version)
{
    assert(data != nullptr);

    std::cout << "registryGlobal: " << name << interface_cstr << version << std::endl;

    std::string interface(interface_cstr);
    auto waylandDisplay = reinterpret_cast<WaylandDisplay*>(data);

    waylandDisplay->registryGlobal(registry, name, interface, version);
}

void WaylandDisplay::registryGlobal(struct wl_registry *registry, uint32_t name, const std::string interface, uint32_t version)
{
    if (interface == std::string(wl_compositor_interface.name)) {
        _compositor = reinterpret_cast<wl_compositor*>(
            wl_registry_bind(_registry, name, &wl_compositor_interface, std::min(version, static_cast<uint32_t>(4))));
        return;
    }
    if (interface == std::string(wl_shm_interface.name)) {
        _shm = reinterpret_cast<wl_shm*>(
            wl_registry_bind(_registry, name, &wl_shm_interface, std::min(version, static_cast<uint32_t>(1))));
        return;
    }
    if (interface == std::string(wl_shell_interface.name)) {
        _shell = reinterpret_cast<wl_shell*>(
            wl_registry_bind(_registry, name, &wl_shell_interface, std::min(version, static_cast<uint32_t>(1))));
        return;
    }
    if (interface == std::string(wl_seat_interface.name)) {
        _seat = reinterpret_cast<wl_seat*>(
            wl_registry_bind(_registry, name, &wl_seat_interface, std::min(version, static_cast<uint32_t>(2))));

        // _pointer = wl_seat_get_pointer(_seat);
        // wl_pointer_add_listener(_pointer, &pointer_listener, NULL);
        return;
    }

    if (interface == std::string(zxdg_shell_v6_interface.name)) {
        std::cout << "wm-base!!!!!" << std::endl;

        _xdgShell = reinterpret_cast<zxdg_shell_v6*>(
            wl_registry_bind(_registry, name, &zxdg_shell_v6_interface, std::min(version, static_cast<uint32_t>(1))));
        return;
    }
}

void WaylandDisplay::registryGlobalRemove(void *data, struct wl_registry *registry, uint32_t name)
{
    std::cout << "registryGlobalRemove: " << name << std::endl;
}