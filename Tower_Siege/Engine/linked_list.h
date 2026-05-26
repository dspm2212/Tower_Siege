/*
 linked_list.h
 -------------
 Implementación simple de lista simplemente enlazada para almacenar
 enemigos (tipo `Enemy`) en el motor. API en estilo C para facilitar
 lectura y pruebas, documentada en estilo académico.

 Todas las funciones son seguras para el uso en este proyecto V1.
*/
#ifndef TOWER_SIEGE_LINKED_LIST_H
#define TOWER_SIEGE_LINKED_LIST_H

#include "GameTypes.h"
#include <vector>

/* Opaque handle a la lista de enemigos */
typedef struct EnemyList EnemyList;

/* Crear una nueva lista dinámica de enemigos.
   Retorna puntero que debe liberarse con `ll_free`.
*/
EnemyList* ll_create();

/* Liberar toda la memoria asociada a la lista */
void ll_free(EnemyList* list);

/* Añadir un enemigo al final de la lista */
void ll_push_back(EnemyList* list, const Enemy* enemy);

/* Convertir la lista en un vector (para interoperabilidad) */
std::vector<Enemy> ll_to_vector(const EnemyList* list);

/* Indica si la lista está vacía */
int ll_empty(const EnemyList* list);

#endif // TOWER_SIEGE_LINKED_LIST_H
