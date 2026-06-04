/*
 * Variant:  Tower Siege
 * Student:  Daniel Santiago Perez Madera - 20231020203
 *
 * BSTree.h
 * --------
 * Binary search tree for Tower structs, ordered by attack_power.
 * C-style API (init/free pattern) matching the course example style.
 * No std::map / std::set / std::multimap used (hard constraint).
 *
 * Operations:
 *   tt_init      -- initialise an empty tree              O(1)
 *   tt_insert    -- insert a tower (by attack_power key)  O(h)
 *   tt_remove_id -- remove tower by id                    O(n scan) + O(h del)
 *   tt_inorder   -- in-order traversal into a vector      O(n)
 *   tt_free      -- release all heap memory               O(n)
 *
 * h = tree height (O(log n) on average for random insertions).
 * n = number of nodes.
 */

#ifndef TOWER_SIEGE_BSTREE_H
#define TOWER_SIEGE_BSTREE_H

#include "GameTypes.h"
#include <vector>

/* Tree node */
typedef struct TowerNode {
    Tower            data;
    struct TowerNode *left;
    struct TowerNode *right;
} TowerNode;

/* Tree handle */
typedef struct {
    TowerNode *root;
    int        size;
} TowerTree;

/* Initialise an empty tree -- O(1) */
void tt_init(TowerTree *tree);

/* Release all nodes (does NOT free the tree struct itself) -- O(n) */
void tt_free(TowerTree *tree);

/* Insert a copy of *tower into the BST keyed on attack_power -- O(h) */
void tt_insert(TowerTree *tree, const Tower *tower);

/* Remove the node whose data.id == id; returns 1 if found, 0 otherwise -- O(n) */
int tt_remove_id(TowerTree *tree, int id);

/* Append towers in ascending attack_power order into *out -- O(n) */
void tt_inorder(const TowerTree *tree, std::vector<Tower> *out);

#endif /* TOWER_SIEGE_BSTREE_H */
