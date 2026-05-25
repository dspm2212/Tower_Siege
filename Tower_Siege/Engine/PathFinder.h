// Team 12
// Variant: Tower Siege
// Students: Ana Pérez, Luis Gómez

#ifndef TOWER_SIEGE_PATHFINDER_H
#define TOWER_SIEGE_PATHFINDER_H

#include "GameTypes.h"
#include <vector>

namespace tower_siege {

std::vector<Position> findShortestPath(
    int grid_size,
    const Position& start,
    const Position& goal,
    const std::vector<Tower>& towers);

Position findGreedyRecommendation(
    int grid_size,
    const Position& castle,
    const std::vector<Tower>& towers,
    const std::vector<Enemy>& enemies);

} // namespace tower_siege

#endif // TOWER_SIEGE_PATHFINDER_H
