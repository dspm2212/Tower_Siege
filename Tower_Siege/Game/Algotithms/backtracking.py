"""
Backtracking pathfinder (documentado).

Función principal:
- find_shortest_path(grid_size, start, goal, towers)
  Retorna la lista de posiciones que forman la ruta más corta
  desde `start` hasta `goal` evitando las torres.
"""
def find_shortest_path(grid_size, start, goal, towers):
    """Encuentra la ruta más corta (por número de pasos) usando DFS.

    start y goal son dicts con 'x','y'. Towers es lista de dicts.
    Devuelve lista de dicts {'x':..,'y':..} que representa el camino.
    """
    def is_tower(x, y):
        return any(t['x'] == x and t['y'] == y for t in towers)

    best = []
    visited = [[False] * grid_size for _ in range(grid_size)]
    visited[start['y']][start['x']] = True
    path = [start.copy()]

    def dfs(cx, cy):
        nonlocal best
        if cx == goal['x'] and cy == goal['y']:
            if not best or len(path) < len(best):
                best = [p.copy() for p in path]
            return
        for dx, dy in ((0, 1), (1, 0), (0, -1), (-1, 0)):
            nx, ny = cx + dx, cy + dy
            if nx < 0 or ny < 0 or nx >= grid_size or ny >= grid_size:
                continue
            if visited[ny][nx]:
                continue
            if is_tower(nx, ny):
                continue
            visited[ny][nx] = True
            path.append({'x': nx, 'y': ny})
            dfs(nx, ny)
            path.pop()
            visited[ny][nx] = False

    dfs(start['x'], start['y'])
    if best and best[0] == start:
        return best[1:]
    return best
