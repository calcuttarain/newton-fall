#include "core/Game.h"
#include "core/LoadLevels.h"
#include "procedural/PerlinNoise.h"
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <box2d/math_functions.h>
#include <stdio.h>
#include <vector>
#include <iostream>

int main() {
    Game game;
    
    try {
        // Load specific level1
        GameConfig level1 = LoadLevels::loadLevelFromJson("game/levels/level1.json");
        game.loadConfig(level1);
        game.run();
    } catch (const std::exception& e) {
        std::cerr << "Error loading level1: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
