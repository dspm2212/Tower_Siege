// Team 12
// Variant: Tower Siege
// Students: Ana Pérez, Luis Gómez

#ifndef TOWER_SIEGE_LINKEDLIST_H
#define TOWER_SIEGE_LINKEDLIST_H

#include "GameTypes.h"

namespace tower_siege {

struct EnemyNode {
    Enemy data;
    EnemyNode* next;
    EnemyNode(const Enemy& enemy) : data(enemy), next(nullptr) {}
};

class EnemyList {
public:
    EnemyList();
    ~EnemyList();

    void push_back(const Enemy& enemy);
    void clear();
    bool empty() const;
    std::vector<Enemy> toVector() const;
    std::vector<Position> toPositionVector() const;

private:
    EnemyNode* head;
    EnemyNode* tail;
};

} // namespace tower_siege

#endif // TOWER_SIEGE_LINKEDLIST_H
