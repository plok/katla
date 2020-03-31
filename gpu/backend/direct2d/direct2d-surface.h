#ifndef KATLA_DIRECT2D_SURFACE_H
#define KATLA_DIRECT2D_SURFACE_H

#include "common/error.h"

#include "../../katla-gpu.h"
#include <memory>

class Direct2D_SurfacePrivate;
class Direct2D_SurfacePrivateDeps;

class KATLA_GPU_DECLSPEC Direct2D_Surface
{
public:
    Direct2D_Surface(const Direct2D_SurfacePrivateDeps& deps);
    virtual ~Direct2D_Surface();

    static Direct2D_Surface create(const Direct2D_SurfacePrivateDeps& deps);
    
private:
    std::unique_ptr<Direct2D_SurfacePrivate> _d;
    const Direct2D_SurfacePrivateDeps& _deps;
};

#endif
