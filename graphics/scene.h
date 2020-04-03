#ifndef KATLA_SCENE_H
#define KATLA_SCENE_H

#include "render-view.h"

// TODO scene / context / canvas etc.... ?

#include <memory>

class Scene;
using ScenePtr = std::shared_ptr<Scene>;

class KATLA_GRAPHICS_DECLSPEC Scene
{
public:
    Scene() = default;
    virtual ~Scene() = default;

    virtual void init() = 0;
    virtual void render() = 0;
    virtual void cleanup() = 0;

    virtual void resize(int width, int height) = 0;
};

#endif