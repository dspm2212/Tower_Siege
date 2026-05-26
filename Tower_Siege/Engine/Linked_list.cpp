/*
 linked_list.cpp
 ----------------
 Implementación de la lista simplemente enlazada definida en
 `linked_list.h`. Implementación mínima, sin dependencias externas.
*/

#include "linked_list.h"
#include <stdlib.h>

struct EnemyNode {
    Enemy data;
    EnemyNode* next;
};

struct EnemyList {
    EnemyNode* head;
    EnemyNode* tail;
};

EnemyList* ll_create() {
    EnemyList* l = (EnemyList*)malloc(sizeof(EnemyList));
    if (!l) return NULL;
    l->head = l->tail = NULL;
    return l;
}

void ll_free(EnemyList* list) {
    if (!list) return;
    EnemyNode* cur = list->head;
    while (cur) {
        EnemyNode* nx = cur->next;
        free(cur);
        cur = nx;
    }
    free(list);
}

void ll_push_back(EnemyList* list, const Enemy* enemy) {
    if (!list || !enemy) return;
    EnemyNode* n = (EnemyNode*)malloc(sizeof(EnemyNode));
    if (!n) return;
    n->data = *enemy;
    n->next = NULL;
    if (!list->head) {
        list->head = list->tail = n;
    } else {
        list->tail->next = n;
        list->tail = n;
    }
}

int ll_empty(const EnemyList* list) {
    return list == NULL || list->head == NULL;
}

std::vector<Enemy> ll_to_vector(const EnemyList* list) {
    std::vector<Enemy> out;
    if (!list) return out;
    EnemyNode* cur = list->head;
    while (cur) { out.push_back(cur->data); cur = cur->next; }
    return out;
}
/*
 Simple enemy linked list implementation.
*/
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
