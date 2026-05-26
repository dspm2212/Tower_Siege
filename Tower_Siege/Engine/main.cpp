/*
 engine/ main.cpp

 Punto de entrada del motor. Lee `data/input.json`, calcula la
 recomendación usando el algoritmo greedy y la ruta óptima con
 backtracking evitando torres; finalmente escribe `data/state.json`.

 Diseño: funciones simples y llamadas a los módulos auxiliares.
*/
#include "GameTypes.h"
#include "JsonIO.h"
#include "linked_list.h"
#include "BSTree.h"
#include "PathFinder.h"
#include <iostream>

int main(void) {
    const std::string inputPath = "data/input.json";
    const std::string statePath = "data/state.json";

    GameInput input;
    if (!readGameInputFromFile(inputPath, input)) {
        std::cerr << "Failed to read input.json\n";
        return 1;
    }

    /* Construir lista de enemigos (custom linked list) */
    EnemyList* el = ll_create();
    for (const Enemy& e : input.enemies) ll_push_back(el, &e);

    /* Construir árbol de torres */
    TowerTree towers;
    for (const Tower& t : input.towers) towers.insert(t);

    GameState state;
    state.recommended_tile = findGreedyRecommendation(input.grid_size, input.castle, input.towers, input.enemies);
    state.optimal_path = findShortestPath(input.grid_size, {0,0}, input.castle, input.towers);
    state.enemy_positions.clear();
    std::vector<Enemy> enemyVec = ll_to_vector(el);
    for (const Enemy& e : enemyVec) state.enemy_positions.push_back({e.x, e.y});

    if (!writeGameStateToFile(statePath, state)) {
        std::cerr << "Failed to write state.json\n";
        ll_free(el);
        return 1;
    }

    ll_free(el);
    return 0;
}
