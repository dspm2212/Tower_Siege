// Team 12
// Variant: Tower Siege
// Students: Ana Pérez, Luis Gómez

#include "JsonIO.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

namespace tower_siege {

static bool parseIntValue(const std::string& source, const std::string& key, int& output) {
    std::size_t pos = source.find(key);
    if (pos == std::string::npos) {
        return false;
    }
    pos = source.find(':', pos);
    if (pos == std::string::npos) {
        return false;
    }
    pos++;
    while (pos < source.size() && std::isspace(static_cast<unsigned char>(source[pos]))) {
        pos++;
    }
    int sign = 1;
    if (source[pos] == '-') {
        sign = -1;
        pos++;
    }
    int value = 0;
    bool found = false;
    while (pos < source.size() && std::isdigit(static_cast<unsigned char>(source[pos]))) {
        found = true;
        value = value * 10 + (source[pos] - '0');
        pos++;
    }
    if (!found) {
        return false;
    }
    output = value * sign;
    return true;
}

static std::string trim(const std::string& text) {
    std::size_t begin = 0;
    while (begin < text.size() && std::isspace(static_cast<unsigned char>(text[begin]))) {
        begin++;
    }
    std::size_t end = text.size();
    while (end > begin && std::isspace(static_cast<unsigned char>(text[end - 1]))) {
        end--;
    }
    return text.substr(begin, end - begin);
}

static bool parseObjectValue(const std::string& objectText, const std::string& key, int& value) {
    return parseIntValue(objectText, key, value);
}

static std::vector<std::string> splitJsonArray(const std::string& source) {
    std::vector<std::string> entries;
    std::size_t pos = 0;
    while (pos < source.size()) {
        std::size_t openBrace = source.find('{', pos);
        if (openBrace == std::string::npos) {
            break;
        }
        std::size_t closeBrace = source.find('}', openBrace);
        if (closeBrace == std::string::npos) {
            break;
        }
        std::string entry = source.substr(openBrace, closeBrace - openBrace + 1);
        entries.push_back(trim(entry));
        pos = closeBrace + 1;
    }
    return entries;
}

bool readFileText(const std::string& path, std::string& output) {
    std::ifstream file(path);
    if (!file.is_open()) {
        return false;
    }
    std::ostringstream builder;
    builder << file.rdbuf();
    output = builder.str();
    return true;
}

bool writeFileText(const std::string& path, const std::string& content) {
    std::ofstream file(path);
    if (!file.is_open()) {
        return false;
    }
    file << content;
    return true;
}

bool parseGameInput(const std::string& text, GameInput& input) {
    if (!parseIntValue(text, "grid_size", input.grid_size)) {
        return false;
    }
    if (!parseObjectValue(text, "\"x\"", input.castle.x)) {
        return false;
    }
    if (!parseObjectValue(text, "\"y\"", input.castle.y)) {
        return false;
    }

    input.towers.clear();
    input.enemies.clear();

    std::size_t towersPos = text.find("\"towers\"");
    if (towersPos != std::string::npos) {
        std::size_t startArray = text.find('[', towersPos);
        std::size_t endArray = text.find(']', startArray);
        if (startArray != std::string::npos && endArray != std::string::npos && endArray > startArray) {
            std::string towersArray = text.substr(startArray + 1, endArray - startArray - 1);
            for (const std::string& entry : splitJsonArray(towersArray)) {
                Tower tower{};
                if (parseObjectValue(entry, "\"x\"", tower.x) && parseObjectValue(entry, "\"y\"", tower.y) && parseObjectValue(entry, "\"power\"", tower.power)) {
                    input.towers.push_back(tower);
                }
            }
        }
    }

    std::size_t enemiesPos = text.find("\"enemies\"");
    if (enemiesPos != std::string::npos) {
        std::size_t startArray = text.find('[', enemiesPos);
        std::size_t endArray = text.find(']', startArray);
        if (startArray != std::string::npos && endArray != std::string::npos && endArray > startArray) {
            std::string enemiesArray = text.substr(startArray + 1, endArray - startArray - 1);
            for (const std::string& entry : splitJsonArray(enemiesArray)) {
                Enemy enemy{};
                if (parseObjectValue(entry, "\"id\"", enemy.id) && parseObjectValue(entry, "\"x\"", enemy.x) && parseObjectValue(entry, "\"y\"", enemy.y) && parseObjectValue(entry, "\"hp\"", enemy.hp)) {
                    input.enemies.push_back(enemy);
                }
            }
        }
    }

    return true;
}

bool readGameInputFromFile(const std::string& path, GameInput& input) {
    std::string text;
    if (!readFileText(path, text)) {
        return false;
    }
    return parseGameInput(text, input);
}

std::string serializeGameState(const GameState& state) {
    std::ostringstream builder;
    builder << "{\n";
    builder << "  \"recommended_tile\": {\"x\": " << state.recommended_tile.x << ", \"y\": " << state.recommended_tile.y << "},\n";
    builder << "  \"optimal_path\": [\n";
    for (std::size_t i = 0; i < state.optimal_path.size(); ++i) {
        const Position& position = state.optimal_path[i];
        builder << "    {\"x\": " << position.x << ", \"y\": " << position.y << "}";
        if (i + 1 < state.optimal_path.size()) {
            builder << ",";
        }
        builder << "\n";
    }
    builder << "  ],\n";
    builder << "  \"enemy_positions\": [\n";
    for (std::size_t i = 0; i < state.enemy_positions.size(); ++i) {
        const Position& position = state.enemy_positions[i];
        builder << "    {\"x\": " << position.x << ", \"y\": " << position.y << "}";
        if (i + 1 < state.enemy_positions.size()) {
            builder << ",";
        }
        builder << "\n";
    }
    builder << "  ]\n";
    builder << "}\n";
    return builder.str();
}

bool writeGameStateToFile(const std::string& path, const GameState& state) {
    std::string content = serializeGameState(state);
    return writeFileText(path, content);
}

} // namespace tower_siege
