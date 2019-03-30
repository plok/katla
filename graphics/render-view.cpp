//
// Created by simon on 10/15/18.
//

#include "render-view.h"

RenderView::RenderView() :
    _renderStats(std::make_shared<RenderStats>())
{

}
RenderView::~RenderView() = default;

void RenderView::init()
{
    for(auto& scene : _scenes) {
        scene->init();
    }
}

void RenderView::render()
{
    _renderStats->beginFrame();

    for(auto& scene : _scenes) {
        scene->render();
    }

    _renderStats->endFrame();
}

void RenderView::resize(int width, int height)
{
    for(auto& scene : _scenes) {
        scene->resize(width, height);
    }
}

void RenderView::cleanup()
{
    for(auto& scene : _scenes) {
        scene->cleanup();
    }
}

void RenderView::addScene(ScenePtr scene)
{
    _scenes.push_back(scene);
}

RenderStatsPtr RenderView::renderStats()
{
    return _renderStats;
}