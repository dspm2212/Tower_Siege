/*
 GameTypes.h
 ----------
 Descripciones estructurales utilizadas por el motor.
 Contiene definiciones simples de datos (structs) que mapean
 directamente el contenido de los archivos JSON `input.json`
 y `state.json` para la versión 1 del juego.

 Nota: sin información de autor ni grupo por requerimiento.
*/
#ifndef TOWER_SIEGE_GAMETYPES_H
#define TOWER_SIEGE_GAMETYPES_H

#include <vector>

/* Posición en la cuadrícula */
struct Position {
    int x; /* columna */
    int y; /* fila    */
};

/* Torre colocada por el jugador */
struct Tower {
    int x;     /* columna */
    int y;     /* fila    */
    int power; /* potencia de ataque */
};

/* Enemigo del juego */
struct Enemy {
    int id; /* identificador único */
    int x;  /* columna */
    int y;  /* fila    */
    int hp; /* puntos de vida */
};

/* Datos de entrada leídos desde input.json */
struct GameInput {
    int grid_size;
    Position castle;
    std::vector<Tower> towers;
    std::vector<Enemy> enemies;
};

/* Estado escrito en state.json */
struct GameState {
    Position recommended_tile;
    std::vector<Position> optimal_path;
    std::vector<Position> enemy_positions;
};

#endif // TOWER_SIEGE_GAMETYPES_H
