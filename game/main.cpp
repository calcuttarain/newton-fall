#include "core/Game.h"
#include "core/LoadLevels.h"
#include "procedural/PerlinNoise.h"
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <box2d/math_functions.h>
#include <stdio.h>
#include <vector>
#include <iostream>

int main(int argc, char* argv[]) {
    try {
        const size_t DEFAULT_LEVEL = 0; // Nivel implicit de încărcat

        auto levels = LoadLevels::loadAllLevels("game/levels");
        
        if (levels.empty()) {
            std::cerr << "No levels found!" << std::endl;
            return 1;
        }

        std::cout << "Found " << levels.size() << " levels (0 to " << (levels.size()-1) << ")" << std::endl;
        
        // Verificăm fiecare nivel încărcat
        for (size_t i = 0; i < levels.size(); i++) {
            std::cout << "Level " << i << " loaded successfully" << std::endl;
        }
        
        size_t selectedLevel = DEFAULT_LEVEL;

        // Verificăm argumentul pentru nivel
        if (argc > 1) {
            try {
                int requestedLevel = std::stoi(argv[1]);
                if (requestedLevel >= 0 && static_cast<size_t>(requestedLevel) < levels.size()) {
                    selectedLevel = static_cast<size_t>(requestedLevel);
                } else {
                    std::cout << "Invalid level " << requestedLevel << ". Valid levels are 0 to " << (levels.size()-1) << ". Using default level " << DEFAULT_LEVEL << std::endl;
                }
            } catch (const std::exception& e) {
                std::cout << "Invalid argument '" << argv[1] << "'. Using default level " << DEFAULT_LEVEL << std::endl;
            }
        }
        
        std::cout << "\nStarting game with level " << selectedLevel << std::endl;
        
        Game game;
        game.loadConfig(levels[selectedLevel]);
        game.run();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
