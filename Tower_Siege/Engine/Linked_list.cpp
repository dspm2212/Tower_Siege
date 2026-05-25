// Team 12
// Variant: Tower Siege
// Students: Ana Pérez, Luis Gómez

#include "LinkedList.h"

namespace tower_siege {

EnemyList::EnemyList()
    : head(nullptr), tail(nullptr) {}

EnemyList::~EnemyList() {
    clear();
}

void EnemyList::push_back(const Enemy& enemy) {
    EnemyNode* node = new EnemyNode(enemy);
    if (!head) {
        head = node;
        tail = node;
    } else {
        tail->next = node;
        tail = node;
    }
}

void EnemyList::clear() {
    EnemyNode* current = head;
    while (current) {
        EnemyNode* next = current->next;
        delete current;
        current = next;
    }
    head = nullptr;
    tail = nullptr;
}

bool EnemyList::empty() const {
    return head == nullptr;
}

std::vector<Enemy> EnemyList::toVector() const {
    std::vector<Enemy> result;
    EnemyNode* current = head;
    while (current) {
        result.push_back(current->data);
        current = current->next;
    }
    return result;
}

std::vector<Position> EnemyList::toPositionVector() const {
    std::vector<Position> result;
    EnemyNode* current = head;
    while (current) {
        result.push_back({current->data.x, current->data.y});
        current = current->next;
    }
    return result;
}

} // namespace tower_siege
