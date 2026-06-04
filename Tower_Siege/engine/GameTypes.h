/*
 * Variant:  Tower Siege
 * Student:  Daniel Santiago Perez Madera - 20231020203
 *
 * GameTypes.h
 * -----------
 * Plain data structs shared by all engine modules.
 * Maps directly to the JSON contract:
 *   input.json  (Python -> C++) and state.json (C++ -> Python).
 * No logic here -- only struct definitions.
 */

#ifndef TOWER_SIEGE_GAME_TYPES_H
#define TOWER_SIEGE_GAME_TYPES_H

#include <string>
#include <vector>

/* Grid position (row = vertical axis, col = horizontal axis) */
typedef struct {
    int row;
    int col;
} Position;

/* Tower placed by the player */
typedef struct {
    int id;
    int row;
    int col;
    int attack_power;
    int radius;        /* attack range in Manhattan distance */
} Tower;

/* Enemy in the current wave */
typedef struct {
    int id;
    int row;
    int col;
    int hp;
    int speed;         /* cells advanced per step (1 = normal, 2 = fast) */
    int path_id;       /* which path this enemy follows (0-based) */
} Enemy;

/* Event emitted during one engine step */
typedef struct {
    std::string type;       /* "tower_attack" | "enemy_killed" | "castle_damaged" | "wave_clear" */
    int         tower_id;   /* -1 if not applicable */
    int         enemy_id;   /* -1 if not applicable */
} Event;

/* Full payload Python -> C++ (input.json) */
typedef struct {
    std::string           command;    /* "step" | "place_tower" | "remove_tower" | "init" */
    int                   rows;
    int                   cols;
    int                   castle_row;
    int                   castle_col;
    int                   castle_hp;
    std::vector<Tower>    towers;
    std::vector<Enemy>    enemies;
    /* One march path per spawn point, each computed by Python backtracking.
       An enemy follows paths[enemy.path_id]. Levels 1-4 use a single path. */
    std::vector<std::vector<Position>> paths;
} GameInput;

/* Full payload C++ -> Python (state.json) */
typedef struct {
    int                castle_hp;
    std::vector<Tower> towers_by_power;  /* in-order BST traversal */
    std::vector<Enemy> enemies;
    std::vector<Event> events;
} GameState;

#endif /* TOWER_SIEGE_GAME_TYPES_H */
