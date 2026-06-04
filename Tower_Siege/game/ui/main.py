"""
 Variant:  Tower Siege
 Student:  Daniel Santiago Perez Madera - 20231020203

 main.py  --  SINGLE ENTRY POINT
 Pygame UI for Tower Siege.

 Responsibilities:
   - Render the grid, towers (with attack range), enemies, castle, paths,
     the greedy-recommended cell, and the HUD.
   - Handle input: left-click place tower, right-click remove, keys to
     select tower type / advance level / pause.
   - Drive 5 levels with timed enemy spawns and a per-level tower budget.
   - Call backtracking.find_path() for each spawn point and
     greedy.recommend_tile() for the placement hint.
   - Call bridge.send_command() to step the C++ engine.

 Run from the project root:
   python game/ui/main.py
"""
import sys
import os


_HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, os.path.join(_HERE, ".."))

import pygame
import bridge
import levels
from algorithms.greedy       import recommend_tile
from algorithms.backtracking import find_path


BOARD_PX  = 560
HUD_WIDTH = 240
MARGIN    = 2
STEP_MS   = 1000
FPS       = 30           

WINDOW_W = BOARD_PX + HUD_WIDTH
WINDOW_H = BOARD_PX

# Colours
C_BG          = (15,  15,  15)
C_CELL        = (238, 238, 238)
C_GRID_LINE   = (180, 180, 180)
C_TOWER       = (50,  50,  50)
C_TOWER_HEAVY = (90,  40,  120)
C_TOWER_TEXT  = (255, 255, 255)
C_RANGE       = (90,  140, 220)
C_TARGET_LINE = (255, 220, 90)
C_CASTLE      = (240, 200, 60)
C_CASTLE_TEXT = (20,  20,  20)
C_ENEMY       = (210, 50,  50)
C_ENEMY_FAST  = (235, 130, 40)
C_ENEMY_HP    = (60,  200, 90)
C_PATH        = (120, 160, 220)
C_RECOMMEND   = (90,  210, 160)   #
C_HUD_TEXT    = (230, 230, 230)
C_WARNING     = (220, 80,  60)
C_SPAWN       = (200, 160, 40)
C_BANNER      = (255, 255, 255)


def cell_px(grid):
    """Pixel size of one cell for a grid of side `grid`."""
    return BOARD_PX // grid


def cell_rect(grid, row, col):
    """pygame.Rect covering grid cell (row, col)."""
    size = cell_px(grid)
    return pygame.Rect(col * size, row * size, size - MARGIN, size - MARGIN)


def cell_center(grid, row, col):
    """Pixel centre of grid cell (row, col)."""
    size = cell_px(grid)
    return (col * size + size // 2, row * size + size // 2)


def draw_text(surface, text, pos, color=C_HUD_TEXT, size=18, bold=False):
    font = pygame.font.SysFont("consolas", size, bold=bold)
    surface.blit(font.render(text, True, color), pos)


def draw_grid(surface, grid):
    for row in range(grid):
        for col in range(grid):
            pygame.draw.rect(surface, C_CELL,      cell_rect(grid, row, col))
            pygame.draw.rect(surface, C_GRID_LINE, cell_rect(grid, row, col), 1)


def draw_recommendation(surface, grid, rec):
    """Fill the greedy-recommended cell with a distinct colour."""
    if rec:
        pygame.draw.rect(surface, C_RECOMMEND, cell_rect(grid, rec['row'], rec['col']))


def draw_paths(surface, grid, paths):
    for path in paths:
        for step in path:
            pygame.draw.rect(surface, C_PATH, cell_rect(grid, step['row'], step['col']), 2)


def draw_tower_range(surface, grid, tower):
    """Draw the Manhattan attack range as a diamond outline."""
    size = cell_px(grid)
    cx, cy = cell_center(grid, tower['row'], tower['col'])
    r = tower.get('radius', 2) * size
    points = [(cx, cy - r), (cx + r, cy), (cx, cy + r), (cx - r, cy)]
    pygame.draw.polygon(surface, C_RANGE, points, 1)


def draw_towers(surface, grid, towers):
    for t in towers:
        color = C_TOWER_HEAVY if t.get('type') == 'heavy' else C_TOWER
        pygame.draw.rect(surface, color, cell_rect(grid, t['row'], t['col']))
        cx = t['col'] * cell_px(grid) + 6
        cy = t['row'] * cell_px(grid) + 6
        draw_text(surface, str(t['attack_power']), (cx, cy), C_TOWER_TEXT, 16, bold=True)
        draw_tower_range(surface, grid, t)


def draw_castle(surface, grid, castle):
    pygame.draw.rect(surface, C_CASTLE, cell_rect(grid, castle['row'], castle['col']))
    cx, cy = cell_center(grid, castle['row'], castle['col'])
    draw_text(surface, "C", (cx - 8, cy - 12), C_CASTLE_TEXT, 26, bold=True)


def draw_spawns(surface, grid, spawns):
    for (row, col) in spawns:
        pygame.draw.rect(surface, C_SPAWN, cell_rect(grid, row, col), 3)


def draw_enemies(surface, grid, enemies):
    size = cell_px(grid)
    for e in enemies:
        fast = e.get('speed', 1) > 1
        color = C_ENEMY_FAST if fast else C_ENEMY
        cx, cy = cell_center(grid, e['row'], e['col'])
        pygame.draw.circle(surface, color, (cx, cy), size // 2 - 8)
        # HP bar (max hp derived from type via speed: fast 45, normal 70)
        max_hp = e.get('max_hp', 45 if fast else 70)
        frac = max(0.0, min(1.0, e['hp'] / float(max_hp)))
        bar_w = size - 14
        bx = e['col'] * size + 7
        by = e['row'] * size + size - 9
        pygame.draw.rect(surface, C_WARNING,  pygame.Rect(bx, by, bar_w, 4))
        pygame.draw.rect(surface, C_ENEMY_HP, pygame.Rect(bx, by, int(bar_w * frac), 4))


def draw_target_lines(surface, grid, towers, enemies, events):
    """Draw a line from each attacking tower to the enemy it hit this step."""
    tower_by_id = {t['id']: t for t in towers}
    enemy_by_id = {e['id']: e for e in enemies}
    for ev in events:
        if ev.get('type') != 'tower_attack':
            continue
        t = tower_by_id.get(ev.get('tower_id'))
        e = enemy_by_id.get(ev.get('enemy_id'))
        if t and e:
            pygame.draw.line(surface, C_TARGET_LINE,
                             cell_center(grid, t['row'], t['col']),
                             cell_center(grid, e['row'], e['col']), 2)


def draw_hud(surface, game):
    hx = BOARD_PX + 8
    draw_text(surface, "TOWER SIEGE", (hx, 10), C_HUD_TEXT, 22, bold=True)
    draw_text(surface, f"Level: {game['level'] + 1}/{levels.level_count()}", (hx, 40), C_HUD_TEXT, 17)

    hp = game['castle_hp']
    draw_text(surface, f"Castle HP: {hp}", (hx, 64),
              C_ENEMY_HP if hp > 40 else C_WARNING, 18, bold=True)

    draw_text(surface, f"Wave: {game['spawned']}/{game['kills_needed']}", (hx, 90), C_HUD_TEXT, 17)
    draw_text(surface, f"Killed: {game['kills']}", (hx, 110), C_HUD_TEXT, 17)
    draw_text(surface, f"On board: {len(game['enemies'])}", (hx, 130), C_HUD_TEXT, 17)
    draw_text(surface, f"Towers: {len(game['towers'])}/{game['budget']}", (hx, 150), C_HUD_TEXT, 17)

    sel = game['selected_type']
    draw_text(surface, f"Tower: {sel}", (hx, 176), C_RECOMMEND, 16, bold=True)
    draw_text(surface, "1=base  2=heavy", (hx, 194), C_HUD_TEXT, 14)
    if not game['heavy_unlocked']:
        draw_text(surface, "(heavy locked)", (hx, 210), C_GRID_LINE, 13)

    draw_text(surface, "---Controls---",        (hx, 232), C_GRID_LINE, 14)
    draw_text(surface, "L-click: place tower",  (hx, 248), C_HUD_TEXT, 13)
    draw_text(surface, "R-click: remove tower", (hx, 264), C_HUD_TEXT, 13)
    draw_text(surface, "SPACE: pause",          (hx, 280), C_HUD_TEXT, 13)

    tbp = game['state'].get('towers_by_power', [])
    if tbp:
        draw_text(surface, "BST by power:", (hx, 304), C_HUD_TEXT, 14)
        for i, t in enumerate(tbp[:7]):
            draw_text(surface, f" [{t['attack_power']}] ({t['row']},{t['col']})",
                      (hx, 322 + i * 15), C_HUD_TEXT, 12)

    if not bridge.engine_available():
        draw_text(surface, "Engine not built!", (hx, WINDOW_H - 20), C_WARNING, 14)

    # Phase banners
    phase = game['phase']
    if phase == 'paused':
        draw_banner(surface, "PAUSED")
    elif phase == 'level_clear':
        draw_banner(surface, "LEVEL CLEAR  -  press N")
    elif phase == 'won':
        draw_banner(surface, "YOU WIN!  -  press R")
    elif phase == 'lost':
        draw_banner(surface, "GAME OVER  -  press R")


def draw_banner(surface, text):
    overlay = pygame.Surface((BOARD_PX, 50))
    overlay.set_alpha(200)
    overlay.fill((0, 0, 0))
    surface.blit(overlay, (0, BOARD_PX // 2 - 25))
    draw_text(surface, text, (20, BOARD_PX // 2 - 12), C_BANNER, 28, bold=True)


def compute_paths(game):
    """One backtracking path per spawn point toward the castle."""
    grid   = game['grid']
    castle = game['castle']
    paths  = []
    for (row, col) in game['spawns']:
        start = {'row': row, 'col': col}
        paths.append(find_path(grid, grid, start, castle, game['towers']))
    return paths


def get_clicked_cell(grid, mouse_pos):
    size = cell_px(grid)
    col = mouse_pos[0] // size
    row = mouse_pos[1] // size
    if 0 <= row < grid and 0 <= col < grid:
        return row, col
    return None


def cell_blocked(game, row, col):
    """True if a cell cannot hold a tower (castle, spawn, or occupied)."""
    if (row, col) == (game['castle']['row'], game['castle']['col']):
        return True
    if (row, col) in game['spawns']:
        return True
    return any(t['row'] == row and t['col'] == col for t in game['towers'])


def placing_blocks_path(game, row, col):
    """True if adding a tower at (row, col) would seal off any spawn."""
    trial = game['towers'] + [{'row': row, 'col': col}]
    grid, castle = game['grid'], game['castle']
    for (sr, sc) in game['spawns']:
        if not find_path(grid, grid, {'row': sr, 'col': sc}, castle, trial):
            return True
    return False


def sync(game, command):
    """Send the current world to the engine and absorb the new state."""
    state = bridge.send_command(
        command, game['grid'], game['grid'], game['castle'],
        game['castle_hp'], game['towers'], game['enemies'], game['paths']
    )
    game['state']   = state
    game['enemies'] = state.get('enemies', [])
    game['castle_hp'] = state.get('castle_hp', game['castle_hp'])
    refresh_recommendation(game)
    return state


def refresh_recommendation(game):
    game['rec'] = recommend_tile(game['grid'], game['grid'],
                                 game['castle'], game['towers'], game['enemies'])


def spawn_one(game):
    """Add one enemy at the next spawn point, cycling type and spawn."""
    n      = game['spawned']
    types  = game['enemy_types']
    spawns = game['spawns']
    type_name = types[n % len(types)]
    path_id   = n % len(spawns)
    spawn     = spawns[path_id]
    enemy = levels.make_enemy(game['next_enemy_id'], type_name, spawn, path_id)
    game['enemies'].append(enemy)
    game['next_enemy_id'] += 1
    game['spawned'] += 1


def start_level(game, index):
    """Reset the world for level `index` and send an init step."""
    cfg = levels.get_level(index)
    grid = cfg['grid']
    game['level']          = index
    game['grid']           = grid
    game['castle']         = {'row': grid - 1, 'col': grid - 1}
    game['spawns']         = list(cfg['spawns'])
    game['enemy_types']    = list(cfg['enemy_types'])
    game['budget']         = cfg['tower_budget']
    game['heavy_unlocked'] = cfg['heavy_unlocked']
    game['kills_needed']   = cfg['kills']
    game['spawn_ms']       = cfg['spawn_ms']
    game['towers']         = []
    game['enemies']        = []
    game['castle_hp']      = 100
    game['kills']          = 0
    game['spawned']        = 0
    game['selected_type']  = 'base'
    game['phase']          = 'playing'
    game['paths']          = compute_paths(game)
    sync(game, "init")



def main():
    pygame.init()
    pygame.display.set_caption("Tower Siege")
    screen = pygame.display.set_mode((WINDOW_W, WINDOW_H))
    clock  = pygame.time.Clock()

    game = {'next_enemy_id': 1, 'next_tower_id': 1}
    start_level(game, 0)

    last_spawn = pygame.time.get_ticks()
    last_step  = pygame.time.get_ticks()
    running    = True

    while running:
        now = pygame.time.get_ticks()

        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False

            elif event.type == pygame.KEYDOWN:
                if event.key == pygame.K_SPACE and game['phase'] in ('playing', 'paused'):
                    game['phase'] = 'paused' if game['phase'] == 'playing' else 'playing'
                elif event.key == pygame.K_1:
                    game['selected_type'] = 'base'
                elif event.key == pygame.K_2 and game['heavy_unlocked']:
                    game['selected_type'] = 'heavy'
                elif event.key == pygame.K_n and game['phase'] == 'level_clear':
                    start_level(game, game['level'] + 1)
                    last_spawn = last_step = now
                elif event.key == pygame.K_r and game['phase'] in ('won', 'lost'):
                    game['next_enemy_id'] = game['next_tower_id'] = 1
                    start_level(game, 0)
                    last_spawn = last_step = now

            elif event.type == pygame.MOUSEBUTTONDOWN and game['phase'] == 'playing':
                cell = get_clicked_cell(game['grid'], event.pos)
                if not cell:
                    continue
                row, col = cell

                if event.button == 1:   # place tower
                    if (len(game['towers']) < game['budget']
                            and not cell_blocked(game, row, col)
                            and not placing_blocks_path(game, row, col)):
                        game['towers'].append(levels.make_tower(
                            game['next_tower_id'], row, col, game['selected_type']))
                        game['next_tower_id'] += 1
                        game['paths'] = compute_paths(game)
                        sync(game, "place_tower")

                elif event.button == 3:  # remove tower
                    hit = [t for t in game['towers'] if t['row'] == row and t['col'] == col]
                    if hit:
                        game['towers'].remove(hit[0])
                        game['paths'] = compute_paths(game)
                        sync(game, "remove_tower")

       
        if game['phase'] == 'playing':
            # Release exactly `kills_needed` enemies for the whole level
            if game['spawned'] < game['kills_needed'] and now - last_spawn >= game['spawn_ms']:
                spawn_one(game)
                last_spawn = now

            if now - last_step >= STEP_MS:
                if game['enemies']:
                    state = sync(game, "step")
                    killed = sum(1 for ev in state.get('events', [])
                                 if ev.get('type') == 'enemy_killed')
                    game['kills'] += killed
                last_step = now

            # Lose if the castle falls. Clear only once the whole wave has been
            # released AND no enemies remain on the board.
            if game['castle_hp'] <= 0:
                game['phase'] = 'lost'
            elif game['spawned'] >= game['kills_needed'] and not game['enemies']:
                game['phase'] = 'won' if game['level'] + 1 >= levels.level_count() else 'level_clear'

        
        grid = game['grid']
        screen.fill(C_BG)
        draw_grid(screen, grid)
        draw_recommendation(screen, grid, game.get('rec'))
        draw_paths(screen, grid, game['paths'])
        draw_spawns(screen, grid, game['spawns'])
        draw_towers(screen, grid, game['towers'])
        draw_castle(screen, grid, game['castle'])
        draw_enemies(screen, grid, game['enemies'])
        draw_target_lines(screen, grid, game['towers'], game['enemies'],
                          game['state'].get('events', []))
        draw_hud(screen, game)

        pygame.display.flip()
        clock.tick(FPS)

    pygame.quit()


if __name__ == "__main__":
    main()
