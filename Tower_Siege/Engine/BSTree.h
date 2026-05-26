/*
 Binary search tree for towers ordered by attack power.
 Manual BST implementation without std::map or std::set.
*/
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
    #ifndef TOWER_SIEGE_BSTREE_H
    #define TOWER_SIEGE_BSTREE_H

    #include "GameTypes.h"
    #include <vector>

    /*
     BSTree.h
     -------
     Árbol binario de búsqueda simple para almacenar torres
     ordenadas por potencia (`power`). Implementación mínima
     separada entre interfaz y fuente (`tree.cpp`).
    */

    struct TowerNode {
        Tower data;
        TowerNode* left;
        TowerNode* right;
        TowerNode(const Tower& t) : data(t), left(nullptr), right(nullptr) {}
    };

    class TowerTree {
    public:
        TowerTree();
        ~TowerTree();
        /* Inserta una torre en el árbol (por potencia) */
        void insert(const Tower& t);
        /* Recorre en-order y devuelve vector ordenado */
        std::vector<Tower> toVector() const;
        /* Libera toda la memoria del árbol */
        void clear();
    private:
        TowerNode* root;
        void insertNode(TowerNode*& node, const Tower& t);
        void gather(TowerNode* node, std::vector<Tower>& out) const;
        void clearNode(TowerNode* node);
    };

    #endif // TOWER_SIEGE_BSTREE_H
