#pragma once

#include "../core/GameConfig.h"
#include "box2d/math_functions.h"
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>

class World {
private:
    b2WorldId worldId;
    const WorldConfig& config;

public:
    World(const WorldConfig& config);
    const WorldConfig& getConfig() const { return config; }
    b2WorldId getWorldId() const;

    ~World();
};
