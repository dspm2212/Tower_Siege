/*
 * Variant:  Tower Siege
 * Student:  Daniel Santiago Perez Madera - 20231020203
 *
 * linked_list.cpp
 * ---------------
 * Implementation of the singly-linked enemy list declared in linked_list.h.
 * Uses malloc/free -- C-style API matching the course examples.
 * No STL list/deque/map used here (hard constraint).
 */

#include "linked_list.h"
#include <stdlib.h>  /* malloc, free, perror, exit */

/* -------------------------------------------------------------------------
 * Internal helpers (static = not visible outside this translation unit)
 * ---------------------------------------------------------------------- */

/* Allocate and initialise a new node; exits on allocation failure */
static EnemyNode *new_node(const Enemy *enemy) {
    EnemyNode *n = (EnemyNode *)malloc(sizeof(EnemyNode));
    if (!n) { perror("linked_list: malloc"); exit(EXIT_FAILURE); }
    n->data = *enemy;
    n->next = NULL;
    return n;
}

/* Find the index of (row, col) in a path; -1 if not found */
static int find_pos_index(const std::vector<Position> &path, int row, int col) {
    for (int i = 0; i < (int)path.size(); i++) {
        if (path[i].row == row && path[i].col == col) return i;
    }
    return -1;
}

/* -------------------------------------------------------------------------
 * Public API
 * ---------------------------------------------------------------------- */

/* Initialise an empty list -- O(1) */
void ll_init(EnemyList *list) {
    list->head = list->tail = NULL;
    list->size = 0;
}

/* Release all nodes (does NOT free the list struct itself) -- O(n) */
void ll_free(EnemyList *list) {
    if (!list) return;
    EnemyNode *cur = list->head;
    while (cur) {
        EnemyNode *next = cur->next;
        free(cur);
        cur = next;
    }
    list->head = list->tail = NULL;
    list->size = 0;
}

/* Append a copy of *enemy to the tail -- O(1) */
void ll_push_back(EnemyList *list, const Enemy *enemy) {
    if (!list || !enemy) return;
    EnemyNode *n = new_node(enemy);
    if (!list->head) {
        list->head = list->tail = n;
    } else {
        list->tail->next = n;
        list->tail        = n;
    }
    list->size++;
}

/* Remove the first node whose data.id == id -- O(n)
   Returns 1 if removed, 0 if not found */
int ll_remove_id(EnemyList *list, int id) {
    if (!list || !list->head) return 0;

    if (list->head->data.id == id) {
        EnemyNode *tmp = list->head;
        list->head = tmp->next;
        if (!list->head) list->tail = NULL;
        free(tmp);
        list->size--;
        return 1;
    }

    EnemyNode *prev = list->head;
    while (prev->next) {
        if (prev->next->data.id == id) {
            EnemyNode *tmp = prev->next;
            prev->next = tmp->next;
            if (!prev->next) list->tail = prev;
            free(tmp);
            list->size--;
            return 1;
        }
        prev = prev->next;
    }
    return 0;
}

/*
 * Advance every enemy along its own path by its speed -- O(n * path_len).
 * If the enemy is not on the path yet, snap it to the start.
 * Fast enemies (speed > 1) take several cells but stop at the goal.
 */
void ll_advance(EnemyList *list, const std::vector<std::vector<Position>> &paths) {
    if (!list) return;
    EnemyNode *cur = list->head;
    while (cur) {
        int pid = cur->data.path_id;
        if (pid < 0 || pid >= (int)paths.size()) { cur = cur->next; continue; }
        const std::vector<Position> &path = paths[pid];
        int len = (int)path.size();
        if (len == 0) { cur = cur->next; continue; }

        int idx = find_pos_index(path, cur->data.row, cur->data.col);
        if (idx < 0) idx = 0;              /* not on path yet -- start there */
        idx += cur->data.speed;            /* move forward by speed */
        if (idx > len - 1) idx = len - 1;   /* clamp at the goal */
        cur->data.row = path[idx].row;
        cur->data.col = path[idx].col;
        cur = cur->next;
    }
}

/* Return 1 if the enemy is at the last position of its path -- O(path_len) */
int ll_at_goal(const Enemy *enemy, const std::vector<std::vector<Position>> &paths) {
    if (!enemy) return 0;
    int pid = enemy->path_id;
    if (pid < 0 || pid >= (int)paths.size()) return 0;
    const std::vector<Position> &path = paths[pid];
    if (path.empty()) return 0;
    return enemy->row == path.back().row && enemy->col == path.back().col;
}

/* Copy all enemies into a std::vector for JSON serialisation -- O(n) */
std::vector<Enemy> ll_to_vector(const EnemyList *list) {
    std::vector<Enemy> out;
    if (!list) return out;
    const EnemyNode *cur = list->head;
    while (cur) {
        out.push_back(cur->data);
        cur = cur->next;
    }
    return out;
}

/* Return 1 if the list has no nodes -- O(1) */
int ll_empty(const EnemyList *list) {
    return !list || list->head == NULL;
}
