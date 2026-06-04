"""
 Variant:  Tower Siege
 Student:  Daniel Santiago Perez Madera - 20231020203

 bridge.py
 ---------
 The ONLY module that talks to the C++ engine.

 Responsibilities:
   - Write input.json  (Python -> C++)  with the current game state
     and the player's requested command.
   - Invoke the compiled engine binary.
   - Read  state.json  (C++ -> Python)  with the updated game state.

 JSON schema (both sides must stay in sync with engine/GameTypes.h):

   input.json
   {
     "command"   : "init" | "step" | "place_tower" | "remove_tower",
     "grid"      : { "rows": 8, "cols": 8 },
     "castle"    : { "row": 7, "col": 7 },
     "castle_hp" : 100,
     "towers"    : [ { "id": 1, "row": 3, "col": 2, "attack_power": 15, "radius": 2 } ],
     "enemies"   : [ { "id": 1, "row": 0, "col": 0, "hp": 100, "speed": 1, "path_id": 0 } ],
     "paths"     : [ [ { "row": 0, "col": 0 }, ... ], ... ]  <- one per spawn point
   }

   state.json
   {
     "castle_hp"       : 100,
     "towers_by_power" : [ { "id": 1, "row": 3, "col": 2, "attack_power": 15, "radius": 2 } ],
     "enemies"         : [ { "id": 1, "row": 1, "col": 0, "hp": 85, "speed": 1, "path_id": 0 } ],
     "events"          : [ { "type": "tower_attack", "tower_id": 1, "enemy_id": 1 } ]
   }
"""

import json
import os
import subprocess

# Resolve paths relative to this file (game/ui/bridge.py)
_HERE       = os.path.dirname(os.path.abspath(__file__))
ROOT_DIR    = os.path.abspath(os.path.join(_HERE, "..", ".."))
INPUT_PATH  = os.path.join(ROOT_DIR, "data", "input.json")
STATE_PATH  = os.path.join(ROOT_DIR, "data", "state.json")
ENGINE_PATH = os.path.join(ROOT_DIR, "engine", "tower_engine.exe")




def _load_json(path):
    """Load and return the parsed JSON at `path`.  Raises on error."""
    with open(path, "r", encoding="utf-8") as f:
        return json.load(f)


def _save_json(path, data):
    """Serialise `data` as JSON and write it to `path` (overwrites)."""
    with open(path, "w", encoding="utf-8") as f:
        json.dump(data, f, indent=2)




def load_state():
    """Return the contents of state.json as a Python dict.

    If state.json is missing or unreadable, return a safe default so the
    UI can still start up before the engine has run for the first time.
    """
    try:
        return _load_json(STATE_PATH)
    except (OSError, json.JSONDecodeError):
        return {
            "castle_hp":       100,
            "towers_by_power": [],
            "enemies":         [],
            "events":          []
        }


def send_command(command, grid_rows, grid_cols, castle, castle_hp,
                 towers, enemies, paths):
    """Write input.json and invoke the engine; return the new state dict.

    Parameters

    command    : str   -- "init" | "step" | "place_tower" | "remove_tower"
    grid_rows  : int
    grid_cols  : int
    castle     : dict  -- {'row': int, 'col': int}
    castle_hp  : int
    towers     : list  -- tower dicts {'id','row','col','attack_power','radius'}
    enemies    : list  -- enemy dicts {'id','row','col','hp','speed','path_id'}
    paths      : list  -- list of paths; each path is a list of {'row','col'}
                          (one path per spawn point, from backtracking)

    Returns
    dict  -- contents of state.json after engine run
    """
    payload = {
        "command":    command,
        "grid":       {"rows": grid_rows, "cols": grid_cols},
        "castle":     castle,
        "castle_hp":  castle_hp,
        "towers":     towers,
        "enemies":    enemies,
        "paths":      paths,
    }
    _save_json(INPUT_PATH, payload)
    _run_engine()
    return load_state()


def engine_available():
    """Return True if the compiled engine binary exists."""
    return os.path.isfile(ENGINE_PATH)



def _run_engine():
    """Launch the engine binary (reads input.json, writes state.json).

    Runs with cwd = ROOT_DIR so relative paths inside main.cpp resolve
    correctly.  Silent on failure -- the UI shows a warning instead.
    """
    if not engine_available():
        return False
    try:
        result = subprocess.run(
            [ENGINE_PATH],
            cwd=os.path.join(ROOT_DIR, "engine"),
            check=True,
            timeout=5
        )
        return result.returncode == 0
    except (subprocess.CalledProcessError, subprocess.TimeoutExpired, OSError):
        return False
