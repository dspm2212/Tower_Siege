// Team 12
// Variant: Tower Siege
// Students: Ana Pérez, Luis Gómez

#ifndef TOWER_SIEGE_GAMETYPES_H
#define TOWER_SIEGE_GAMETYPES_H

#include <vector>

namespace tower_siege {

struct Position {
    int x;
    int y;
};

struct Tower {
    int x;
    int y;
    int power;
};

struct Enemy {
    int id;
    int x;
    int y;
    int hp;
};

struct GameInput {
    int grid_size;
    Position castle;
    std::vector<Tower> towers;
    std::vector<Enemy> enemies;
};

struct GameState {
    Position recommended_tile;
    std::vector<Position> optimal_path;
    std::vector<Position> enemy_positions;
};

} // namespace tower_siege

#endif // TOWER_SIEGE_GAMETYPES_H
