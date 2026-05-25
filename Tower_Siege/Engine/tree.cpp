// Team 12
// Variant: Tower Siege
// Students: Ana Pérez, Luis Gómez

#include "BSTree.h"

namespace tower_siege {

TowerTree::TowerTree()
    : root(nullptr) {}

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
