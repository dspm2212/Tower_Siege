"""
 Variant:  Tower Siege
 Student:  Daniel Santiago Perez Madera - 20231020203

 backtracking.py
 Pathfinder using DFS with backtracking.

 Finds the shortest path (fewest steps) from `start` to `goal` on the
 grid, avoiding cells occupied by towers.  The result is sent to the C++
 engine inside input.json so the engine can advance enemies along it.

 Algorithm: DFS that explores all routes and keeps the shortest one found.
 Complexity: O(4^(rows*cols)) worst case, but the grid is small (8x8 = 64
 cells) so this is acceptable for a single call per game event.

 Pure function -- no side effects, no I/O.
Called by game/ui/main.py whenever towers change or a step begins.
"""

def find_path(grid_rows, grid_cols, start, goal, towers):
    """Find the shortest path from start to goal avoiding towers.

    Parameters
    grid_rows : int
    grid_cols : int
    start     : dict -- {'row': int, 'col': int}
    goal      : dict -- {'row': int, 'col': int}
    towers    : list -- each item has 'row' and 'col'

    Returns
    list of dict  [{'row': r, 'col': c}, ...]  -- includes start and goal.
    Returns []    if no path exists.
    """
    tower_set = {(t['row'], t['col']) for t in towers}

    best    = []
    visited = [[False] * grid_cols for _ in range(grid_rows)]
    path    = [{'row': start['row'], 'col': start['col']}]
    visited[start['row']][start['col']] = True

    def dfs(row, col):
        nonlocal best
        if row == goal['row'] and col == goal['col']:
            if not best or len(path) < len(best):
                best = [p.copy() for p in path]
            return
        for dr, dc in ((1, 0), (0, 1), (-1, 0), (0, -1)):
            nr, nc = row + dr, col + dc
            if nr < 0 or nr >= grid_rows or nc < 0 or nc >= grid_cols:
                continue
            if visited[nr][nc]:
                continue
            if (nr, nc) in tower_set:
                continue
            steps_remaining = abs(nr - goal['row']) + abs(nc - goal['col'])
            if best and len(path) + steps_remaining >= len(best):
                continue
            visited[nr][nc] = True
            path.append({'row': nr, 'col': nc})
            dfs(nr, nc)
            path.pop()
            visited[nr][nc] = False

    dfs(start['row'], start['col'])
    return best
