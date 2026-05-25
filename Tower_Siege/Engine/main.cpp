// Team 12
// Variant: Tower Siege
// Students: Ana Pérez, Luis Gómez

#include "GameTypes.h"
#include "JsonIO.h"
#include "LinkedList.h"
#include "BSTree.h"
#include "PathFinder.h"
#include <iostream>

int main() {
    const std::string inputPath = "data/input.json";
    const std::string statePath = "data/state.json";

    tower_siege::GameInput input;
    if (!tower_siege::readGameInputFromFile(inputPath, input)) {
        std::cerr << "Failed to read input file: " << inputPath << "\n";
        return 1;
    }

    tower_siege::EnemyList enemyList;
    for (const auto& enemy : input.enemies) {
        enemyList.push_back(enemy);
    }

    tower_siege::TowerTree towerTree;
    for (const auto& tower : input.towers) {
        towerTree.insert(tower);
    }

    tower_siege::GameState state;
    state.recommended_tile = tower_siege::findGreedyRecommendation(
        input.grid_size,
        input.castle,
        input.towers,
        input.enemies);

    state.optimal_path = tower_siege::findShortestPath(
        input.grid_size,
        {0, 0},
        input.castle,
        input.towers);

    state.enemy_positions = enemyList.toPositionVector();

    if (!tower_siege::writeGameStateToFile(statePath, state)) {
        std::cerr << "Failed to write state file: " << statePath << "\n";
        return 1;
    }

    return 0;
}
