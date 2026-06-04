/*
 * Variant:  Tower Siege
 * Student:  Daniel Santiago Perez Madera - 20231020203
 *
 * main.cpp
 * --------
 * Engine entry point.
 *
 * Flow:
 *   1. Read  data/input.json   (written by Python bridge.py)
 *   2. Rebuild the enemy linked list and tower BST from input state
 *   3. Apply the requested command:
 *        "init" / "place_tower" / "remove_tower" -- echo state back
 *        "step" -- advance enemies along their paths, towers attack the
 *                  nearest enemy in range, resolve castle damage
 *   4. Write data/state.json   (read by Python bridge.py)
 *
 * Greedy recommendation and pathfinding live in Python (game/algorithms/).
 * The engine receives the pre-computed paths from Python via input.json.
 *
 * Data structures:
 *   EnemyList (linked_list.cpp) -- the active enemy wave
 *   TowerTree (tree.cpp)        -- towers ordered by attack_power (BST)
 */

#include "GameTypes.h"
#include "JsonIO.h"
#include "linked_list.h"
#include "BSTree.h"
#include <iostream>
#include <cstdlib>   /* abs */

static const std::string INPUT_PATH = "../data/input.json";
static const std::string STATE_PATH = "../data/state.json";

/* Damage dealt to the castle by each enemy that reaches it */
static const int CASTLE_DAMAGE = 10;

/* Manhattan distance between two grid cells */
static int manhattan(int r1, int c1, int r2, int c2) {
    return std::abs(r1 - r2) + std::abs(c1 - c2);
}

/* Build an Event in one line (keeps the call sites short) */
static Event make_event(const std::string &type, int tower_id, int enemy_id) {
    Event ev;
    ev.type     = type;
    ev.tower_id = tower_id;
    ev.enemy_id = enemy_id;
    return ev;
}

/* --------------------------------------------------------------------------
 * apply_tower_attacks:
 *   Each tower attacks the NEAREST living enemy inside its radius, dealing
 *   attack_power damage to that single enemy. Then dead enemies are removed.
 *   Emits "tower_attack" and "enemy_killed" events.
 * ----------------------------------------------------------------------- */
static void apply_tower_attacks(EnemyList *enemies, const TowerTree *towers, GameState *state) {
    std::vector<Tower> tower_vec;
    tt_inorder(towers, &tower_vec);

    /* Each tower picks one target: the closest enemy within its radius */
    for (const Tower &t : tower_vec) {
        EnemyNode *target = NULL;
        int best_dist = t.radius + 1;
        for (EnemyNode *cur = enemies->head; cur; cur = cur->next) {
            if (cur->data.hp <= 0) continue;
            int dist = manhattan(t.row, t.col, cur->data.row, cur->data.col);
            if (dist <= t.radius && dist < best_dist) {
                best_dist = dist;
                target = cur;
            }
        }
        if (target) {
            target->data.hp -= t.attack_power;
            state->events.push_back(make_event("tower_attack", t.id, target->data.id));
        }
    }

    /* Remove enemies that died this step */
    std::vector<Enemy> snapshot = ll_to_vector(enemies);
    for (const Enemy &e : snapshot) {
        if (e.hp <= 0) {
            state->events.push_back(make_event("enemy_killed", -1, e.id));
            ll_remove_id(enemies, e.id);
        }
    }
}

/* --------------------------------------------------------------------------
 * resolve_castle_damage:
 *   Enemies that reached the end of their path damage the castle and leave.
 *   Returns the total damage dealt this step.
 * ----------------------------------------------------------------------- */
static int resolve_castle_damage(EnemyList *enemies,
                                 const std::vector<std::vector<Position>> &paths,
                                 GameState *state) {
    int total = 0;
    std::vector<Enemy> snapshot = ll_to_vector(enemies);
    for (const Enemy &e : snapshot) {
        if (ll_at_goal(&e, paths)) {
            total += CASTLE_DAMAGE;
            state->events.push_back(make_event("castle_damaged", -1, e.id));
            ll_remove_id(enemies, e.id);
        }
    }
    return total;
}

/* --------------------------------------------------------------------------
 * main
 * ----------------------------------------------------------------------- */
int main(void) {
    /* --- 1. Read input.json --- */
    GameInput input;
    if (!read_input(INPUT_PATH, input)) {
        std::cerr << "engine: cannot read " << INPUT_PATH << "\n";
        return 1;
    }

    /* --- 2. Build data structures from input --- */
    EnemyList enemies;
    ll_init(&enemies);
    for (const Enemy &e : input.enemies)
        ll_push_back(&enemies, &e);

    TowerTree towers;
    tt_init(&towers);
    for (const Tower &t : input.towers)
        tt_insert(&towers, &t);

    /* --- 3. Apply command --- */
    GameState state;
    state.castle_hp = input.castle_hp;
    state.events.clear();

    if (input.command == "step") {
        ll_advance(&enemies, input.paths);
        apply_tower_attacks(&enemies, &towers, &state);

        int dmg = resolve_castle_damage(&enemies, input.paths, &state);
        state.castle_hp -= dmg;
        if (state.castle_hp < 0) state.castle_hp = 0;

        if (ll_empty(&enemies))
            state.events.push_back(make_event("wave_clear", -1, -1));
    }
    /* "init" / "place_tower" / "remove_tower": just echo the state back */

    /* --- 4. Fill state output --- */
    state.enemies = ll_to_vector(&enemies);
    tt_inorder(&towers, &state.towers_by_power);

    /* --- 5. Write state.json --- */
    if (!write_state(STATE_PATH, state)) {
        std::cerr << "engine: cannot write " << STATE_PATH << "\n";
        ll_free(&enemies);
        tt_free(&towers);
        return 1;
    }

    ll_free(&enemies);
    tt_free(&towers);
    return 0;
}
