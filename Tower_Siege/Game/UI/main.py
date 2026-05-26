"""
Tower Siege UI module.
Renders the grid, towers, castle, enemies, and path.
"""
"""
UI main module (Pygame)
------------------------
Renderiza la cuadrícula 8x8, permite colocar torres con el ratón
y muestra la recomendación del algoritmo greedy y el camino óptimo.

Comunica con el motor mediante archivos JSON usando `bridge.py`.
"""
import os
import pygame
from bridge import load_input, save_input, save_state, load_state, run_engine

# Constantes de la interfaz
GRID_SIZE = 8
CELL_SIZE = 70
MARGIN = 2
WINDOW_WIDTH = GRID_SIZE * CELL_SIZE + 200
WINDOW_HEIGHT = GRID_SIZE * CELL_SIZE

WHITE = (245, 245, 245)
BLACK = (20, 20, 20)
LIGHT_GREY = (200, 200, 200)
BLUE = (60, 120, 220)
RED = (220, 60, 60)
GREEN = (90, 180, 120)
GOLD = (240, 200, 80)
DARK_GREY = (50, 50, 50)


def create_default_input():
    return {
        "grid_size": GRID_SIZE,
        "castle": {"x": 7, "y": 7},
        "towers": [],
        "enemies": [{"id": 1, "x": 0, "y": 0, "hp": 100}]
    }


def create_default_state():
    return {
        "recommended_tile": {"x": 2, "y": 2},
        "optimal_path": [],
        "enemy_positions": [{"x": 0, "y": 0}]
    }


def load_or_create_files():
    """Asegura que existan `input.json` y `state.json`. Devuelve (input, state)."""
    base = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", ".."))
    input_path = os.path.join(base, "data", "input.json")
    state_path = os.path.join(base, "data", "state.json")
    if not os.path.exists(input_path):
        save_input(create_default_input())
    if not os.path.exists(state_path):
        save_state(create_default_state())
    return load_input(), load_state()


def draw_text(surface, text, position, color=BLACK, size=18):
    font = pygame.font.Font(None, size)
    label = font.render(text, True, color)
    surface.blit(label, position)


def draw_grid(surface):
    for row in range(GRID_SIZE):
        for col in range(GRID_SIZE):
            rect = pygame.Rect(col * CELL_SIZE, row * CELL_SIZE, CELL_SIZE - MARGIN, CELL_SIZE - MARGIN)
            pygame.draw.rect(surface, WHITE, rect)
            pygame.draw.rect(surface, LIGHT_GREY, rect, 1)


def draw_towers(surface, towers):
    for tower in towers:
        rect = pygame.Rect(tower["x"] * CELL_SIZE, tower["y"] * CELL_SIZE, CELL_SIZE - MARGIN, CELL_SIZE - MARGIN)
        pygame.draw.rect(surface, DARK_GREY, rect)
        draw_text(surface, str(tower["power"]), (tower["x"] * CELL_SIZE + 8, tower["y"] * CELL_SIZE + 8), WHITE, 20)


def draw_castle(surface, castle):
    rect = pygame.Rect(castle["x"] * CELL_SIZE, castle["y"] * CELL_SIZE, CELL_SIZE - MARGIN, CELL_SIZE - MARGIN)
    pygame.draw.rect(surface, GOLD, rect)
    draw_text(surface, "C", (castle["x"] * CELL_SIZE + 16, castle["y"] * CELL_SIZE + 16), BLACK, 28)


def draw_enemies(surface, enemy_positions):
    for enemy in enemy_positions:
        rect = pygame.Rect(enemy["x"] * CELL_SIZE + 14, enemy["y"] * CELL_SIZE + 14, CELL_SIZE - 28, CELL_SIZE - 28)
        pygame.draw.ellipse(surface, RED, rect)


def draw_path(surface, path):
    for position in path:
        rect = pygame.Rect(position["x"] * CELL_SIZE + 18, position["y"] * CELL_SIZE + 18, CELL_SIZE - 36, CELL_SIZE - 36)
        pygame.draw.rect(surface, BLUE, rect, 2)


def draw_recommendation(surface, recommended):
    rect = pygame.Rect(recommended["x"] * CELL_SIZE, recommended["y"] * CELL_SIZE, CELL_SIZE - MARGIN, CELL_SIZE - MARGIN)
    pygame.draw.rect(surface, GREEN, rect, 4)


def get_cell_from_mouse(position):
    x, y = position
    col = x // CELL_SIZE
    row = y // CELL_SIZE
    if 0 <= col < GRID_SIZE and 0 <= row < GRID_SIZE:
        return col, row
    return None


def update_game_state(game_input):
    save_input(game_input)
    run_engine()
    return load_state()


def main():
    pygame.init()
    pygame.display.set_caption("Tower Siege")
    screen = pygame.display.set_mode((WINDOW_WIDTH, WINDOW_HEIGHT))
    clock = pygame.time.Clock()

    game_input = create_default_input()
    game_state = create_default_state()
    save_input(game_input)
    save_state(game_state)
    game_state = update_game_state(game_input)

    running = True
    while running:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False
                break

            if event.type == pygame.MOUSEBUTTONDOWN and event.button == 1:
                cell = get_cell_from_mouse(event.pos)
                if cell:
                    col, row = cell
                    existing_tower = any(t["x"] == col and t["y"] == row for t in game_input["towers"])
                    castle = game_input["castle"]
                    if not existing_tower and not (col == castle["x"] and row == castle["y"]) and not (col == 0 and row == 0):
                        new_power = 15 + len(game_input["towers"]) * 5
                        game_input["towers"].append({"x": col, "y": row, "power": new_power})
                        game_state = update_game_state(game_input)

        screen.fill(BLACK)
        draw_grid(screen)
        draw_path(screen, game_state.get("optimal_path", []))
        draw_towers(screen, game_input["towers"])
        draw_castle(screen, game_input["castle"])
        draw_enemies(screen, game_state.get("enemy_positions", []))
        draw_recommendation(screen, game_state.get("recommended_tile", {"x": 0, "y": 0}))

        draw_text(screen, "Tower Siege - Version 1", (GRID_SIZE * CELL_SIZE + 12, 12), WHITE, 24)
        draw_text(screen, "Click empty cells to place towers.", (GRID_SIZE * CELL_SIZE + 12, 48), WHITE, 18)
        draw_text(screen, f"Towers: {len(game_input['towers'])}", (GRID_SIZE * CELL_SIZE + 12, 86), WHITE, 18)
        draw_text(screen, f"Recommended: ({game_state['recommended_tile']['x']}, {game_state['recommended_tile']['y']})", (GRID_SIZE * CELL_SIZE + 12, 116), WHITE, 18)
        draw_text(screen, f"Path length: {len(game_state.get('optimal_path', []))}", (GRID_SIZE * CELL_SIZE + 12, 146), WHITE, 18)

        if not os.path.exists(os.path.join(os.path.dirname(__file__), "..", "..", "Engine", "tower_siege_engine.exe")):
            draw_text(screen, "Build engine before running: ", (GRID_SIZE * CELL_SIZE + 12, 190), RED, 18)
            draw_text(screen, "See README for instructions.", (GRID_SIZE * CELL_SIZE + 12, 212), RED, 18)

        pygame.display.flip()
        clock.tick(30)

    pygame.quit()


if __name__ == "__main__":
    main()
