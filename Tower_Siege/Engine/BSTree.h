// Team 12
// Variant: Tower Siege
// Students: Daniel Perez

#ifndef TOWER_SIEGE_BSTREE_H
#define TOWER_SIEGE_BSTREE_H

#include "GameTypes.h"
#include <vector>

namespace tower_siege {

struct TowerNode {
    Tower data;
    TowerNode* left;
    TowerNode* right;
    TowerNode(const Tower& tower) : data(tower), left(nullptr), right(nullptr) {}
};

class TowerTree {
public:
    TowerTree();
    ~TowerTree();

    void insert(const Tower& tower);
    std::vector<Tower> toVector() const;
    void clear();

private:
    TowerNode* root;
    void insertNode(TowerNode*& node, const Tower& tower);
    void gatherSorted(TowerNode* node, std::vector<Tower>& output) const;
    void clearNode(TowerNode* node);
};

} // namespace tower_siege

#endif // TOWER_SIEGE_BSTREE_H
