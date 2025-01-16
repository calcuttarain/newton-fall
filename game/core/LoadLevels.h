#pragma once
#include "GameConfig.h"
#include <string>
#include <vector>
#include <filesystem>
#include <nlohmann/json.hpp>

class LoadLevels {
public:
    static std::vector<GameConfig> loadAllLevels(const std::string& levelsPath);
    static GameConfig loadLevelFromJson(const std::string& jsonPath);

private:
    static void parseDisplayConfig(const nlohmann::json& j, DisplayConfig& config);
    static void parseWorldConfig(const nlohmann::json& j, WorldConfig& config);
    static void parseGroundConfig(const nlohmann::json& j, GroundConfig& config);
    static void parseSquareConfig(const nlohmann::json& j, SquareConfig& config);
    static void parseWallConfig(const nlohmann::json& j, WallConfig& config);
    static void parseGameConfig(const nlohmann::json& j, GameConfig& config);
};
