"""
 Variant:  Tower Siege
 Student:  Daniel Santiago Perez Madera - 20231020203

 levels.py

 Pure data + small helpers describing the 5 game levels.
 No pygame, no I/O -- main.py reads this to configure each level.

 Each level is a dict:
   grid           : grid side length (rows == cols)
   kills          : enemies the player must kill to clear the level
   spawn_ms       : milliseconds between enemy spawns
   enemy_types    : list of enemy-type names spawned (cycled in order)
   tower_budget   : max towers the player may place this level
   spawns         : list of (row, col) spawn points (1 normally, 2 in L5)
   heavy_unlocked : True if the heavy tower can be placed this level
"""

TOWER_TYPES = {
    'base':  {'attack_power': 15, 'radius': 2},
    'heavy': {'attack_power': 40, 'radius': 3},
}


ENEMY_TYPES = {
    'normal': {'hp': 70, 'speed': 1},
    'fast':   {'hp': 45, 'speed': 2},
}


LEVELS = [
    {   # level 1
        'grid': 8,  'kills': 5,  'spawn_ms': 4000,
        'enemy_types': ['normal'],
        'tower_budget': 4,  'heavy_unlocked': False,
        'spawns': [(0, 0)],
    },
    {   # level 2
        'grid': 8,  'kills': 6,  'spawn_ms': 3500,
        'enemy_types': ['normal'],
        'tower_budget': 4,  'heavy_unlocked': False,
        'spawns': [(0, 0)],
    },
    {   # Level 3
        'grid': 10, 'kills': 7,  'spawn_ms': 3500,
        'enemy_types': ['normal'],
        'tower_budget': 5,  'heavy_unlocked': True,
        'spawns': [(0, 0)],
    },
    {   # Level 4
        'grid': 10, 'kills': 8,  'spawn_ms': 3000,
        'enemy_types': ['normal', 'fast'],
        'tower_budget': 5,  'heavy_unlocked': True,
        'spawns': [(0, 0)],
    },
    {   # Level 5
        'grid': 10, 'kills': 10, 'spawn_ms': 3000,
        'enemy_types': ['normal', 'fast'],
        'tower_budget': 6,  'heavy_unlocked': True,
        'spawns': [(0, 0), (0, 9)],
    },
]


def level_count():
    """Return how many levels exist."""
    return len(LEVELS)


def get_level(index):
    """Return the config dict for level `index` (0-based)."""
    return LEVELS[index]


def make_enemy(enemy_id, type_name, spawn, path_id):
    """Build one enemy dict from its type, spawn cell, and path id."""
    stats = ENEMY_TYPES[type_name]
    return {
        'id':      enemy_id,
        'row':     spawn[0],
        'col':     spawn[1],
        'hp':      stats['hp'],
        'max_hp':  stats['hp'],   
        'speed':   stats['speed'],
        'path_id': path_id,
        'type':    type_name,
    }


def make_tower(tower_id, row, col, type_name):
    """Build one tower dict from its type and position."""
    stats = TOWER_TYPES[type_name]
    return {
        'id':           tower_id,
        'row':          row,
        'col':          col,
        'attack_power': stats['attack_power'],
        'radius':       stats['radius'],
        'type':         type_name,   
    }
