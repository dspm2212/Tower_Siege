# Tower Siege — Versión 1

Proyecto académico simple: juego tipo tower-defense (versión inicial).

Estructura del proyecto

- `project/engine` — código C++ del motor (lee `input.json`, escribe `state.json`).
- `project/game/ui` — UI en Python usando Pygame (render, interacción, bridge JSON).
- `project/game/algorithms` — implementaciones Python de los algoritmos (greedy, backtracking).
- `project/data` — archivos `input.json` y `state.json` para comunicación entre UI y motor.

Reglas importantes

- Solo se usan bibliotecas estándar en C++.
- Python permite `pygame` y librerías estándar.
- Integración únicamente por archivos JSON en `project/data`.

Archivos relevantes

- Motor C++ (carpeta `project/engine`):
  - `main.cpp` — punto de entrada (lee `../data/input.json`, escribe `../data/state.json`).
  - `JsonIO.cpp/.h` — parseo y serialización JSON (simple).
  - `linked_list.cpp/.h` — lista enlazada personalizada para enemigos.
  - `BSTree.h`, `tree.cpp` — BST para almacenar torres.
  - `PathFinder.cpp/.h` — backtracking y greedy en C++.

- UI Python (carpeta `project/game/ui`):
  - `main.py` — UI Pygame (barra lateral, colocar torres con click izquierdo).
  - `bridge.py` — funciones para leer/escribir JSON y ejecutar el motor.

- Algoritmos Python (carpeta `project/game/algorithms`):
  - `greedy.py` — recomendación greedy (documentado).
  - `backtracking.py` — backtracking pathfinder.

Compilación del motor (Windows)

Recomendado: MinGW/GCC o Microsoft Visual C++ (Developer Command Prompt).

Con MinGW (PowerShell):
```powershell
cd "c:\Users\HP\Documents\GitHub\Tower-Siege\Tower_Siege\project\engine"
g++ -std=c++17 -O2 main.cpp JsonIO.cpp linked_list.cpp tree.cpp PathFinder.cpp -o tower_siege_engine.exe
```

Con MSVC (Developer Command Prompt):
```powershell
cd "c:\Users\HP\Documents\GitHub\Tower-Siege\Tower_Siege\project\engine"
cl /EHsc /std:c++17 main.cpp JsonIO.cpp linked_list.cpp tree.cpp PathFinder.cpp /Fe:tower_siege_engine.exe
```

Ejecución de la UI (Python)

1. Instala `pygame` si no lo tienes:
```powershell
python -m pip install pygame
```
2. Ejecuta la UI:
```powershell
cd "c:\Users\HP\Documents\GitHub\Tower-Siege\Tower_Siege\project\game\ui"
python main.py
```

Notas

- El ejecutable `tower_siege_engine.exe` debe estar en `project/engine` para que `bridge.py` lo encuentre y pueda ejecutar el motor.
- Si prefieres ejecutar manualmente el motor para regenerar `state.json`, desde `project/engine` ejecuta el binario; leerá `../data/input.json` y escribirá `../data/state.json`.
- Los archivos JSON tienen el formato controlado descrito en el enunciado; modifica `input.json` antes de ejecutar si quieres probar otras configuraciones.

Problemas comunes

- Si la UI no muestra la recomendación o el camino, asegúrate de compilar el motor y que `tower_siege_engine.exe` esté en `project/engine`.
- Si el motor no compila: verifica que tu compilador soporte C++17 y que estés en la carpeta correcta al compilar.

¿Quieres que añada scripts de build (PowerShell `.ps1`) o un `Makefile` para facilitar la compilación? Si usas WSL/Linux puedo incluir comandos alternativos.
