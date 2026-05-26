/*
 Tower BST implementation.
/*
 tree.cpp
 --------
 Implementación de las funciones declaradas en `BSTree.h`.
 Mantiene operaciones básicas: constructor, destructor,
 inserción, recorrido in-order y limpieza de memoria.
*/

#include "BSTree.h"
#include <vector>

TowerTree::TowerTree()
    : root(nullptr) {}

TowerTree::~TowerTree() {
    clear();
}

void TowerTree::insert(const Tower& t) {
    insertNode(root, t);
}

std::vector<Tower> TowerTree::toVector() const {
    std::vector<Tower> out;
    gather(root, out);
    return out;
}

void TowerTree::clear() {
    clearNode(root);
    root = nullptr;
}

void TowerTree::insertNode(TowerNode*& node, const Tower& t) {
    if (!node) { node = new TowerNode(t); return; }
    if (t.power < node->data.power) insertNode(node->left, t);
    else insertNode(node->right, t);
}

void TowerTree::gather(TowerNode* node, std::vector<Tower>& out) const {
    if (!node) return;
    gather(node->left, out);
    out.push_back(node->data);
    gather(node->right, out);
}

void TowerTree::clearNode(TowerNode* node) {
    if (!node) return;
    clearNode(node->left);
    clearNode(node->right);
    delete node;
}

TowerTree::~TowerTree() {
    clear();
}

void TowerTree::insert(const Tower& tower) {
    insertNode(root, tower);
}

std::vector<Tower> TowerTree::toVector() const {
    std::vector<Tower> items;
    gatherSorted(root, items);
    return items;
}

void TowerTree::clear() {
    clearNode(root);
    root = nullptr;
}

void TowerTree::insertNode(TowerNode*& node, const Tower& tower) {
    if (!node) {
        node = new TowerNode(tower);
        return;
    }

    if (tower.power < node->data.power) {
        insertNode(node->left, tower);
    } else {
        insertNode(node->right, tower);
    }
}

void TowerTree::gatherSorted(TowerNode* node, std::vector<Tower>& output) const {
    if (!node) {
        return;
    }
    gatherSorted(node->left, output);
    output.push_back(node->data);
    gatherSorted(node->right, output);
}

void TowerTree::clearNode(TowerNode* node) {
    if (!node) {
        return;
    }
    clearNode(node->left);
    clearNode(node->right);
    delete node;
}

} // namespace tower_siege
