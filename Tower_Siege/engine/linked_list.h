/*
 * Variant:  Tower Siege
 * Student:  Daniel Santiago Perez Madera - 20231020203
 *
 * linked_list.h
 * -------------
 * Singly-linked list of Enemy structs for the engine wave queue.
 * C-style API: init/free pattern, snake_case, no STL containers for
 * the list itself (hard constraint -- no std::list).
 *
 * Operations:
 *   ll_init         -- initialise an empty list           O(1)
 *   ll_push_back    -- append enemy to tail               O(1)
 *   ll_remove_id    -- remove enemy by id                 O(n)
 *   ll_advance      -- move each enemy along its own path  O(n)
 *   ll_to_vector    -- copy enemies into a std::vector    O(n)
 *   ll_empty        -- true if list has no nodes          O(1)
 *   ll_free         -- release all heap memory            O(n)
 */

#ifndef TOWER_SIEGE_LINKED_LIST_H
#define TOWER_SIEGE_LINKED_LIST_H

#include "GameTypes.h"
#include <vector>

/* Linked-list node */
typedef struct EnemyNode {
    Enemy            data;
    struct EnemyNode *next;
} EnemyNode;

/* List handle -- head/tail for O(1) append, size for convenience */
typedef struct {
    EnemyNode *head;
    EnemyNode *tail;
    int        size;
} EnemyList;

/* Initialise list (no heap allocation needed) */
void ll_init(EnemyList *list);

/* Release all nodes; does NOT free the list struct itself */
void ll_free(EnemyList *list);

/* Append a copy of *enemy to the tail -- O(1) */
void ll_push_back(EnemyList *list, const Enemy *enemy);

/* Remove the node whose data.id == id; returns 1 if found, 0 otherwise -- O(n) */
int ll_remove_id(EnemyList *list, int id);

/*
 * Advance every enemy along its own march path by its speed.
 * `paths` holds one path per spawn point; an enemy follows paths[path_id].
 * Each enemy moves `speed` cells toward the castle. Enemies already at the
 * last path position stay there (the caller detects castle arrivals).
 *                                                           O(n * path_len)
 */
void ll_advance(EnemyList *list, const std::vector<std::vector<Position>> &paths);

/* Return 1 if `enemy` is at the last position of its path -- O(path_len) */
int ll_at_goal(const Enemy *enemy, const std::vector<std::vector<Position>> &paths);

/* Copy all live enemies into a std::vector (for JSON serialisation) -- O(n) */
std::vector<Enemy> ll_to_vector(const EnemyList *list);

/* Return 1 if the list is empty -- O(1) */
int ll_empty(const EnemyList *list);

#endif /* TOWER_SIEGE_LINKED_LIST_H */
