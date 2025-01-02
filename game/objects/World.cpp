#include "World.h"
#include "box2d/id.h"

World::World(const GameConfig& config) : config(config) {
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = config.gravity;
    worldId = b2CreateWorld(&worldDef);
}

b2WorldId World::getWorldId() const { return this->worldId; }

World::~World() {
    b2DestroyWorld(this->worldId);
    this->worldId = b2_nullWorldId;
}
