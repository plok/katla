//
// Created by simon on 10/15/18.
//

#ifndef KATLA_RENDERSTATS_H
#define KATLA_RENDERSTATS_H

#include <chrono>
#include <optional>
#include <memory>

class RenderStats;
using RenderStatsPtr = std::shared_ptr<RenderStats>;

class RenderStats {
public:
    struct Stats {
        Stats(long lastDuration, long lastRenderInterval) :
                LastDuration(lastDuration),
                LastRenderInterval(lastRenderInterval)
        {}

        const long LastDuration = 0;
        const long LastRenderInterval = 0;
    };

    void beginFrame();
    void endFrame();

    Stats stats();
private:
    std::optional<std::chrono::high_resolution_clock::time_point> _lastBegin;
    std::optional<std::chrono::high_resolution_clock::time_point> _lastRender;

    long _lastDuration;
    long _lastRenderInterval;
};


#endif //SEISMOGRAPH_RENDERSTATS_H
