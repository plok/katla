#ifndef KATLA_RENDER_VIEW_H
#define KATLA_RENDER_VIEW_H

#include "scene.h"
#include "render-stats.h"

#include <vector>

class RenderView
{
public:
    RenderView();
    virtual ~RenderView();

    void init();
    void cleanup();

    void resize(int width, int height);

    void render();

    void addScene(ScenePtr scene);

    RenderStatsPtr renderStats();

private:
    std::vector<ScenePtr> _scenes;

    RenderStatsPtr _renderStats;
};

#endif