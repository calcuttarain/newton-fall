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
    try {
        auto levels = LoadLevels::loadAllLevels("game/levels");
        
        if (levels.empty()) {
            std::cerr << "No levels found!" << std::endl;
            return 1;
        }

        // Afișare corectă: pentru nivele 0-4, levels.size() este 5
        std::cout << "Found " << levels.size() << " levels (0 to " << (levels.size()-1) << ")" << std::endl;
        
        // Verificăm fiecare nivel încărcat
        for (size_t i = 0; i < levels.size(); i++) {
            std::cout << "Level " << i << " loaded successfully" << std::endl;
        }
        
        const size_t selectedLevel = 0;  // Acesta este al 5-lea nivel (indice 4)
        std::cout << "\nLoading level " << selectedLevel << std::endl;
        
        if (selectedLevel >= levels.size()) {
            std::cerr << "Invalid level index! Available levels are 0 to " << (levels.size()-1) << std::endl;
            return 1;
        }
        
        Game game;
        game.loadConfig(levels[selectedLevel]);  // Acum va încărca corect nivelul 4
        game.run();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
