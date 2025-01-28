#include "LoadLevels.h"
#include <fstream>
#include <iostream>

std::vector<GameConfig> LoadLevels::loadAllLevels(const std::string& levelsPath) {
    std::vector<std::pair<int, std::string>> levelFiles;
    std::vector<GameConfig> levels;
    
    try {
        // Colectăm toate fișierele .json și le indexăm
        for (const auto& entry : std::filesystem::directory_iterator(levelsPath)) {
            if (entry.path().extension() == ".json") {
                std::string filename = entry.path().stem().string();
                try {
                    int index = std::stoi(filename);
                    std::cout << "Found level file: " << filename << ".json" << " with index " << index << std::endl;
                    levelFiles.push_back({index, entry.path().string()});
                } catch (const std::exception& e) {
                    std::cerr << "Skipping " << filename << ": not a number" << std::endl;
                }
            }
        }

        // Sortăm explicit folosind indexul numeric
        std::sort(levelFiles.begin(), levelFiles.end(), 
            [](const auto& a, const auto& b) { return a.first < b.first; });

        // Cream un vector de dimensiunea corectă
        levels.resize(levelFiles.size());

        // Încărcăm nivelurile în pozițiile corecte
        for (const auto& [index, path] : levelFiles) {
            std::cout << "Loading level " << index << " from: " << path << std::endl;
            if (index >= 0 && static_cast<size_t>(index) < levels.size()) {
                levels[index] = loadLevelFromJson(path);
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error loading levels: " << e.what() << std::endl;
    }
    
    return levels;
}

GameConfig LoadLevels::loadLevelFromJson(const std::string& jsonPath) {
    std::ifstream file(jsonPath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + jsonPath);
    }

    nlohmann::json j;
    file >> j;
    GameConfig config;
    parseGameConfig(j, config);
    return config;
}

void LoadLevels::parseDisplayConfig(const nlohmann::json& j, DisplayConfig& config) {
    if (j.contains("backgroundSize")) {
        config.backgroundSize = sf::Vector2f(j["backgroundSize"][0], j["backgroundSize"][1]);
    }
    if (j.contains("windowSize")) {
        config.windowSize = sf::Vector2f(j["windowSize"][0], j["windowSize"][1]);
    }
    if (j.contains("viewSize")) {
        config.viewSize = sf::Vector2f(j["viewSize"][0], j["viewSize"][1]);
    }
    if (j.contains("fps")) {
        config.fps = j["fps"];
    }
}

void LoadLevels::parseWorldConfig(const nlohmann::json& j, WorldConfig& config) {
    if (j.contains("gravity")) {
        config.gravity = {static_cast<float>(j["gravity"][0]), 
                         static_cast<float>(j["gravity"][1])};
    }
}

void LoadLevels::parseGroundConfig(const nlohmann::json& j, GroundConfig& config) {
    if (j.contains("position")) {
        config.position = {static_cast<float>(j["position"][0]), 
                          static_cast<float>(j["position"][1])};
    }
    if (j.contains("size")) {
        config.size = {static_cast<float>(j["size"][0]), 
                      static_cast<float>(j["size"][1])};
    }
}

void LoadLevels::parseSquareConfig(const nlohmann::json& j, SquareConfig& config) {
    if (j.contains("position")) {
        config.position = {static_cast<float>(j["position"][0]), 
                          static_cast<float>(j["position"][1])};
    }
    if (j.contains("size")) {
        config.size = {static_cast<float>(j["size"][0]), 
                      static_cast<float>(j["size"][1])};
    }
    if (j.contains("density")) config.density = j["density"];
    if (j.contains("friction")) config.friction = j["friction"];
    if (j.contains("bounciness")) config.bounciness = j["bounciness"];
    if (j.contains("damageThreshold")) config.damageThreshold = j["damageThreshold"];
    if (j.contains("maxHealth")) config.maxHealth = j["maxHealth"];
    if (j.contains("invincibilityTime")) config.invincibilityTime = j["invincibilityTime"];
}

void LoadLevels::parseWallConfig(const nlohmann::json& j, WallConfig& config) {
    if (j.contains("height")) config.height = j["height"];
    if (j.contains("spacing")) config.spacing = j["spacing"];
    if (j.contains("leftWallSeed")) config.leftWallSeed = j["leftWallSeed"];
    if (j.contains("rightWallSeed")) config.rightWallSeed = j["rightWallSeed"];
    if (j.contains("nodesCount")) config.nodesCount = j["nodesCount"];
    if (j.contains("samplesCount")) config.samplesCount = j["samplesCount"];
    if (j.contains("octavesCount")) config.octavesCount = j["octavesCount"];
    if (j.contains("amplitude")) config.amplitude = j["amplitude"];
    if (j.contains("persistance")) config.persistance = j["persistance"];

    // Adăugăm parsarea parametrilor pentru path
    if (j.contains("pathSeed")) config.pathSeed = j["pathSeed"];
    if (j.contains("pathNodesCount")) config.pathNodesCount = j["pathNodesCount"];
    if (j.contains("pathSamplesCount")) config.pathSamplesCount = j["pathSamplesCount"];
    if (j.contains("pathOctavesCount")) config.pathOctavesCount = j["pathOctavesCount"];
    if (j.contains("pathAmplitude")) config.pathAmplitude = j["pathAmplitude"];
    if (j.contains("pathPersistance")) config.pathPersistance = j["pathPersistance"];
}

void LoadLevels::parseGameConfig(const nlohmann::json& j, GameConfig& config) {
    if (j.contains("display")) parseDisplayConfig(j["display"], config.displayConfig);
    if (j.contains("world")) parseWorldConfig(j["world"], config.worldConfig);
    if (j.contains("ground")) parseGroundConfig(j["ground"], config.groundConfig);
    if (j.contains("square")) parseSquareConfig(j["square"], config.squareConfig);
    if (j.contains("wall")) parseWallConfig(j["wall"], config.wallConfig);
}
