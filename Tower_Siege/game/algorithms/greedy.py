"""
 Variant:  Tower Siege
 Student:  Daniel Santiago Perez Madera - 20231020203

 greedy.py
 ---------
 Greedy tower-placement advisor.

 Given the current grid, existing towers, the castle position, and the
 active enemy list, this module recommends the single empty cell that
 maximises the incoming-threat score.

 Score for a candidate cell (row, col):
   + sum of (8 - dist) * (enemy_hp / 100)  for every alive enemy
     within Manhattan distance <= 7
   + (8 - castle_dist) * 1.5               proximity bonus to the castle

 Pure function -- no side effects, no I/O.
 Called by game/ui/main.py to drive the green highlight on the grid.
"""


def recommend_tile(grid_rows, grid_cols, castle, towers, enemies):
    """Return the cell {'row': r, 'col': c} with the highest threat score.

    Parameters
    grid_rows : int
    grid_cols : int
    castle    : dict  -- keys 'row', 'col'
    towers    : list  -- each item: {'row', 'col', 'attack_power', 'id'}
    enemies   : list  -- each item: {'id', 'row', 'col', 'hp'}

    Returns
    dict  {'row': int, 'col': int}
    """
    occupied   = {(t['row'], t['col']) for t in towers}
    castle_row = castle['row']
    castle_col = castle['col']

    best_cell  = {'row': 0, 'col': 0}
    best_score = -1.0

    for row in range(grid_rows):
        for col in range(grid_cols):
            if row == 0 and col == 0:
                continue
            if row == castle_row and col == castle_col:
                continue
            if (row, col) in occupied:
                continue

            score = 0.0

            for e in enemies:
                if e.get('hp', 0) <= 0:
                    continue
                dist = abs(e['row'] - row) + abs(e['col'] - col)
                if dist <= 7:
                    score += (8 - dist) * (e['hp'] / 100.0)

            castle_dist = abs(row - castle_row) + abs(col - castle_col)
            score += max(0, 8 - castle_dist) * 1.5

            if score > best_score:
                best_score = score
                best_cell  = {'row': row, 'col': col}

    return best_cell
