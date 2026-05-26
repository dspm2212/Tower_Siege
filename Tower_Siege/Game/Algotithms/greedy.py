"""
Greedy recommendation algorithm (documented).

Funciones:
- recommend_tile(grid_size, castle, towers, enemies):
    Calcula la mejor celda vacía para colocar la siguiente torre
    usando una puntuación de amenaza basada en enemigos cercanos
    y distancia al castillo.
"""
def recommend_tile(grid_size, castle, towers, enemies):
    """Devuelve un dict {'x':int,'y':int} con la celda recomendada.

    Parámetros:
    - grid_size: tamaño de la cuadrícula (int)
    - castle: dict con claves 'x' y 'y'
    - towers: lista de dicts con 'x','y','power'
    - enemies: lista de dicts con 'id','x','y','hp'
    """
    best = {'x': 0, 'y': 0}
    best_score = -1.0
    for y in range(grid_size):
        for x in range(grid_size):
            if (x == 0 and y == 0) or (x == castle['x'] and y == castle['y']):
                continue
            if any(t['x'] == x and t['y'] == y for t in towers):
                continue
            score = 0.0
            for e in enemies:
                if e.get('hp', 0) <= 0:
                    continue
                d = abs(e['x'] - x) + abs(e['y'] - y)
                if d <= 7:
                    score += (8 - d) * (e['hp'] / 100.0)
            castle_dist = abs(x - castle['x']) + abs(y - castle['y'])
            score += max(0, 8 - castle_dist) * 1.5
            if score > best_score:
                best_score = score
                best = {'x': x, 'y': y}
    return best
