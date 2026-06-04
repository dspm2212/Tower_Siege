# Tower Siege — Game module

This folder contains the playable game: the **C++ engine**, the **Python/Pygame UI**,
the **algorithms**, and the **JSON data** that bridges both sides. For the project
overview, course information, and documentation, see the
[main repository README](../README.md).

## Project structure

```
Tower_Siege/
    engine/                 C++ engine
        main.cpp            reads input.json -> writes state.json
        linked_list.cpp/.h  hand-rolled linked list (enemy wave)
        tree.cpp / BSTree.h hand-rolled BST (towers by attack power)
        JsonIO.cpp/.h       hand-rolled JSON parser/serialiser
        GameTypes.h         shared struct definitions
    game/
        levels.py           level definitions (data only)
        algorithms/
            greedy.py       greedy placement advisor
            backtracking.py DFS pathfinder
        ui/
            main.py         Pygame UI 
            bridge.py       writes input.json, runs engine, reads state.json
    data/
        input.json          Python -> C++
        state.json          C++ -> Python
    requirements.txt
```

## Build the C++ engine

Requires a C++17 compiler (MinGW g++ or MSVC cl).

**MinGW / g++ (recommended):**
```powershell
cd Tower_Siege\engine
g++ -std=c++17 -O2 main.cpp JsonIO.cpp linked_list.cpp tree.cpp -o tower_engine.exe
```

**MSVC (Developer Command Prompt):**
```powershell
cd Tower_Siege\engine
cl /EHsc /std:c++17 main.cpp JsonIO.cpp linked_list.cpp tree.cpp /Fe:tower_engine.exe
```

## Run the game

```powershell
# Install the only Python dependency
python -m pip install pygame

# Run from the Tower_Siege/ directory (single entry point)
python game/ui/main.py
```


## Controls

| Input | Action |
|---|---|
| Left-click grid cell | Place the selected tower  |
| Right-click tower | Remove the tower |
| `1` | Select base tower |
| `2` | Select heavy tower (from level 3) |
| SPACE | Pause / unpause |
| N | Advance to the next level (when "LEVEL CLEAR") |
| R | Restart from level 1 (when "YOU WIN" / "GAME OVER") |

## Levels

There are **5 levels**. The number under "Wave" is the exact number of enemies
released; a level is cleared once the whole wave has been released **and** no
enemies remain on the board. If the castle HP reaches 0, the game is over.

| Level | Grid | Wave (enemies) | Spawn rate | Enemy types | Tower budget | New content |
|---|---|---|---|---|---|---|
| 1 | 8×8   | 5  | 4.0s | normal        | 4 | the basics |
| 2 | 8×8   | 6  | 3.5s | normal        | 4 | more enemies |
| 3 | 10×10 | 7  | 3.5s | normal        | 5 | heavy tower + bigger grid |
| 4 | 10×10 | 8  | 3.0s | normal + fast | 5 | fast enemy |
| 5 | 10×10 | 10 | 3.0s | normal + fast | 6 | two spawn points / paths |

- **Towers**: base (damage 15, range 2) and heavy (damage 40, range 3, unlocked at
  level 3). Each tower attacks the nearest enemy inside its range every step; the
  range is drawn as a diamond and a line points to the current target.
- **Enemies**: normal (70 HP, speed 1) and fast (45 HP, speed 2).
- **Greedy hint**: the recommended placement cell is filled in a distinct colour.

## JSON contract

`input.json` (Python writes, C++ reads):
```json
{
  "command":   "step",
  "grid":      { "rows": 8, "cols": 8 },
  "castle":    { "row": 7, "col": 7 },
  "castle_hp": 100,
  "towers":    [ { "id": 1, "row": 3, "col": 2, "attack_power": 15, "radius": 2 } ],
  "enemies":   [ { "id": 1, "row": 0, "col": 0, "hp": 70, "speed": 1, "path_id": 0 } ],
  "paths":     [ [ { "row": 0, "col": 0 }, { "row": 1, "col": 0 } ] ]
}
```

`state.json` (C++ writes, Python reads):
```json
{
  "castle_hp":       95,
  "towers_by_power": [ { "id": 1, "row": 3, "col": 2, "attack_power": 15, "radius": 2 } ],
  "enemies":         [ { "id": 1, "row": 1, "col": 0, "hp": 55, "speed": 1, "path_id": 0 } ],
  "events":          [ { "type": "tower_attack", "tower_id": 1, "enemy_id": 1 } ]
}
```

`paths` is a list of paths (one per spawn point); `towers_by_power` comes from the
in-order BST traversal, demonstrating the tree works.
