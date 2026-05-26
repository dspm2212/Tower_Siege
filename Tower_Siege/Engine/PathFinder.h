/*
 PathFinder.h
 ------------
 Declaraciones de los algoritmos: búsqueda de camino por backtracking
 y recomendación greedy para la siguiente torre.
*/
#ifndef TOWER_SIEGE_PATHFINDER_H
#define TOWER_SIEGE_PATHFINDER_H

#include "GameTypes.h"
#include <vector>

/* Encuentra el camino más corto desde `start` hasta `goal` evitando
   las posiciones ocupadas por `towers`.
   Retorna un vector de `Position` que representa el camino.
*/
std::vector<Position> findShortestPath(
    int grid_size,
    const Position& start,
    const Position& goal,
    const std::vector<Tower>& towers);

/* Devuelve la celda recomendada para la siguiente torre según una
   puntuación de amenaza basada en enemigos cercanos y distancia al castillo. */
Position findGreedyRecommendation(
    int grid_size,
    const Position& castle,
    const std::vector<Tower>& towers,
    const std::vector<Enemy>& enemies);

#endif // TOWER_SIEGE_PATHFINDER_H
