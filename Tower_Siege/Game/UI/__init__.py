# Team 12
# Variant: Tower Siege
# Students: Ana Pérez, Luis Gómez
'

Set-Content -Path "README.md" -Value @'
# Tower Siege

Version 1 of the Tower Siege academic game project.

## Structure

- `Engine/` - C++ engine with custom singly linked list and BST
- `data/` - JSON bridge for engine and UI
- `Game/UI/` - Python + Pygame user interface

## Build

Compile the engine with a C++17 compiler:

```bat
cd Tower_Siege
g++ Engine\main.cpp Engine\Linked_list.cpp Engine\tree.cpp -std=c++17 -O2 -o Engine\tower_siege_engine.exe
```

## Run

1. Build the C++ engine.
2. Run the Python UI:

```bat
cd Tower_Siege\Game\UI
python main.py
```

## Notes

- The engine reads `data/input.json` and writes `data/state.json`.
- The UI places towers, shows the recommended greedy tile, and renders the computed path.
