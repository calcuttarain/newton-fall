#include "core/Game.h"
#include "procedural/PerlinNoise.h"
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <box2d/math_functions.h>
#include <stdio.h>
#include <vector>
#include <iostream>

int main() {
    Game game;
    game.run();
    return 0;
}
