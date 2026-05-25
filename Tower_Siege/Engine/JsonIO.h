// Team 12
// Variant: Tower Siege
// Students: Ana Pérez, Luis Gómez

#ifndef TOWER_SIEGE_JSONIO_H
#define TOWER_SIEGE_JSONIO_H

#include "GameTypes.h"
#include <string>

namespace tower_siege {

bool readFileText(const std::string& path, std::string& output);
bool writeFileText(const std::string& path, const std::string& content);
bool parseGameInput(const std::string& text, GameInput& input);
bool readGameInputFromFile(const std::string& path, GameInput& input);
std::string serializeGameState(const GameState& state);
bool writeGameStateToFile(const std::string& path, const GameState& state);

} // namespace tower_siege

#endif // TOWER_SIEGE_JSONIO_H
