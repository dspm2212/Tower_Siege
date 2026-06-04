# Tower Siege

Tower Siege is a grid-based **tower-defense game**: waves of enemies march from a
spawn point toward a castle, and the player places towers on the grid to stop them
before they break through. Each tower automatically attacks the nearest enemy within
its range, and a greedy advisor highlights the best cell to place the next tower.

## About this project

This project was created for the course **Computer Sciences I** of the **Computing
Engineering** program at the **Universidad Distrital Francisco José de Caldas**. It
demonstrates the practical use of classic data structures and algorithms inside a
playable game:

- **Algorithms**
  - *Greedy*: recommends the highest-threat tile for the next tower.
  - *Backtracking (DFS)*: computes the enemy march path around the placed towers.
- **Data structures (hand-written, no STL containers)**
  - *Singly linked list*: stores and advances the active enemy wave (C++ engine).
  - *Binary Search Tree (BST)*: stores towers ordered by attack power (C++ engine).

**Student:** Daniel Santiago Pérez Madera — 20231020203

## Documentation

The supporting documentation for this project is included in the repository
(`docs/` folder):

- **Technical Report** — design, architecture, and implementation details.
- **Poster** — visual summary of the project.
- **Paper** — written report of the work.

## Architecture

Three layers communicate **only through JSON files on disk** (no sockets, no bindings):

| Layer | Language | Responsibility |
|---|---|---|
| Core engine | C++ | Linked list (enemies) + BST (towers). Reads `input.json`, writes `state.json`. |
| Algorithms + UI | Python | Greedy + backtracking logic; Pygame rendering, HUD, and input. |
| Bridge | JSON files | `input.json` (Python → C++) and `state.json` (C++ → Python). |

## Repository layout

```
Tower-Siege/
    Tower_Siege/    the game (C++ engine + Python UI). See its README to build & play.
    docs/           technical report, poster, and paper.
```

## How to play

Build instructions, how to run the game, controls, the level guide, and the JSON
contract live in the game's own README:

**→ [`Tower_Siege/README.md`](Tower_Siege/README.md)**

## Design constraints

- C++ engine: standard library only, no external libraries (JSON parsed by hand).
- Linked list and BST implemented from scratch (no `std::list` / `std::map` / `std::set`).
- Python UI: `pygame` + the standard library only.
- JSON files are the only bridge between C++ and Python.
- Single entry point: `game/ui/main.py`.