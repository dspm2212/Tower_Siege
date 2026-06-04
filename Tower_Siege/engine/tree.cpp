/*
 * Variant:  Tower Siege
 * Student:  Daniel Santiago Perez Madera - 20231020203
 *
 * tree.cpp
 * --------
 * Implementation of the BST declared in BSTree.h.
 * Uses malloc/free -- C-style API consistent with linked_list.cpp.
 * No std::map / std::set used (hard constraint).
 *
 * The tree is ordered by tower.attack_power.
 * Duplicate attack_power values go to the right subtree.
 */

#include "BSTree.h"
#include <stdlib.h>  /* malloc, free, perror, exit */

/* -------------------------------------------------------------------------
 * Internal helpers (static)
 * ---------------------------------------------------------------------- */

/* Allocate and initialise a leaf node */
static TowerNode *new_node(const Tower *tower) {
    TowerNode *n = (TowerNode *)malloc(sizeof(TowerNode));
    if (!n) { perror("tree: malloc"); exit(EXIT_FAILURE); }
    n->data  = *tower;
    n->left  = n->right = NULL;
    return n;
}

/* Recursive insert by attack_power key -- O(h) */
static TowerNode *insert_node(TowerNode *node, const Tower *tower) {
    if (!node) return new_node(tower);
    if (tower->attack_power < node->data.attack_power)
        node->left  = insert_node(node->left,  tower);
    else
        node->right = insert_node(node->right, tower);
    return node;
}

/* In-order traversal -- fills *out in ascending attack_power order -- O(n) */
static void inorder_node(const TowerNode *node, std::vector<Tower> *out) {
    if (!node) return;
    inorder_node(node->left,  out);
    out->push_back(node->data);
    inorder_node(node->right, out);
}

/* Find the node with the minimum key in a subtree (used for BST deletion) */
static TowerNode *min_node(TowerNode *node) {
    while (node->left) node = node->left;
    return node;
}

/*
 * Remove a node by attack_power key and id (both must match).
 * Returns the updated subtree root.
 */
static TowerNode *remove_node(TowerNode *node, int attack_power, int id, int *removed) {
    if (!node) return NULL;

    if (attack_power < node->data.attack_power) {
        node->left  = remove_node(node->left,  attack_power, id, removed);
    } else if (attack_power > node->data.attack_power) {
        node->right = remove_node(node->right, attack_power, id, removed);
    } else {
        /* attack_power matches; check id */
        if (node->data.id == id) {
            *removed = 1;
            if (!node->left) {
                TowerNode *tmp = node->right;
                free(node);
                return tmp;
            }
            if (!node->right) {
                TowerNode *tmp = node->left;
                free(node);
                return tmp;
            }
            /* Two children: replace with in-order successor */
            TowerNode *successor = min_node(node->right);
            node->data  = successor->data;
            node->right = remove_node(node->right, successor->data.attack_power,
                                      successor->data.id, removed);
        } else {
            /* Same power, different id -- search both subtrees */
            node->left  = remove_node(node->left,  attack_power, id, removed);
            if (!*removed)
                node->right = remove_node(node->right, attack_power, id, removed);
        }
    }
    return node;
}

/* Scan the tree to find the attack_power of the node with the given id -- O(n) */
static int find_power_by_id(const TowerNode *node, int id, int *power) {
    if (!node) return 0;
    if (node->data.id == id) { *power = node->data.attack_power; return 1; }
    return find_power_by_id(node->left, id, power) ||
           find_power_by_id(node->right, id, power);
}

/* Free all nodes in a subtree -- O(n) */
static void free_node(TowerNode *node) {
    if (!node) return;
    free_node(node->left);
    free_node(node->right);
    free(node);
}

/* -------------------------------------------------------------------------
 * Public API
 * ---------------------------------------------------------------------- */

void tt_init(TowerTree *tree) {
    tree->root = NULL;
    tree->size = 0;
}

void tt_free(TowerTree *tree) {
    if (!tree) return;
    free_node(tree->root);
    tree->root = NULL;
    tree->size = 0;
}

/* Insert a copy of *tower into the tree -- O(h) */
void tt_insert(TowerTree *tree, const Tower *tower) {
    if (!tree || !tower) return;
    tree->root = insert_node(tree->root, tower);
    tree->size++;
}

/* Remove by id (scans to find the power key first) -- O(n) total */
int tt_remove_id(TowerTree *tree, int id) {
    if (!tree || !tree->root) return 0;
    int power   = 0;
    int removed = 0;
    if (!find_power_by_id(tree->root, id, &power)) return 0;
    tree->root = remove_node(tree->root, power, id, &removed);
    if (removed) tree->size--;
    return removed;
}

/* Append towers sorted by ascending attack_power into *out -- O(n) */
void tt_inorder(const TowerTree *tree, std::vector<Tower> *out) {
    if (!tree || !out) return;
    inorder_node(tree->root, out);
}
