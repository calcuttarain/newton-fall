#include <box2d/box2d.h>

// Test - simulare simplă pentru 3 secunde
int main() {
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = (b2Vec2){0.0f, -10.0f};
    b2WorldId worldId = b2CreateWorld(&worldDef);
    float timeStep = 1.0f / 60.0f;
    for (int i = 0; i < 180; ++i) { 
        b2World_Step(worldId, timeStep, 8);
    }
    b2DestroyWorld(worldId);

    return 0;
}
