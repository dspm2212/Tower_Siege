// Team 12
// Variant: Tower Siege
// Students: Ana Pérez, Luis Gómez

#include "PathFinder.h"
#include <algorithm>
#include <cmath>

namespace tower_siege {

static bool isTowerAt(const std::vector<Tower>& towers, int x, int y) {
    for (const Tower& tower : towers) {
        if (tower.x == x && tower.y == y) {
            return true;
        }
    }
    return false;
}

static int manhattanDistance(const Position& a, const Position& b) {
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

static bool isValidCell(int grid_size, int x, int y) {
    return x >= 0 && x < grid_size && y >= 0 && y < grid_size;
}

static void backtrackPath(
    int grid_size,
    const Position& goal,
    const std::vector<Tower>& towers,
    std::vector<std::vector<bool>>& visited,
    std::vector<Position>& current,
    std::vector<Position>& best,
    const Position& currentPosition) {

    if (currentPosition.x == goal.x && currentPosition.y == goal.y) {
        if (best.empty() || current.size() < best.size()) {
            best = current;
        }
        return;
    }

    static const int dx[4] = {0, 1, 0, -1};
    static const int dy[4] = {1, 0, -1, 0};

    for (int index = 0; index < 4; ++index) {
        int nextX = currentPosition.x + dx[index];
        int nextY = currentPosition.y + dy[index];

        if (!isValidCell(grid_size, nextX, nextY)) {
            continue;
        }
        if (visited[nextY][nextX]) {
            continue;
        }
        if (isTowerAt(towers, nextX, nextY)) {
            continue;
        }

        visited[nextY][nextX] = true;
        current.push_back({nextX, nextY});
        backtrackPath(grid_size, goal, towers, visited, current, best, {nextX, nextY});
        current.pop_back();
        visited[nextY][nextX] = false;
    }
}

std::vector<Position> findShortestPath(
    int grid_size,
    const Position& start,
    const Position& goal,
    const std::vector<Tower>& towers) {

    std::vector<Position> bestPath;
    if (isTowerAt(towers, start.x, start.y) || isTowerAt(towers, goal.x, goal.y)) {
        return bestPath;
    }

    std::vector<std::vector<bool>> visited(grid_size, std::vector<bool>(grid_size, false));
    visited[start.y][start.x] = true;
    std::vector<Position> current;
    current.push_back(start);
    backtrackPath(grid_size, goal, towers, visited, current, bestPath, start);

    if (!bestPath.empty() && bestPath.front().x == start.x && bestPath.front().y == start.y) {
        bestPath.erase(bestPath.begin());
    }

    return bestPath;
}

Position findGreedyRecommendation(
    int grid_size,
    const Position& castle,
    const std::vector<Tower>& towers,
    const std::vector<Enemy>& enemies) {

    Position bestTile = {0, 0};
    double bestScore = -1.0;

    for (int y = 0; y < grid_size; ++y) {
        for (int x = 0; x < grid_size; ++x) {
            if ((x == 0 && y == 0) || (x == castle.x && y == castle.y)) {
                continue;
            }
            if (isTowerAt(towers, x, y)) {
                continue;
            }

            Position cell = {x, y};
            double score = 0.0;

            for (const Enemy& enemy : enemies) {
                if (enemy.hp <= 0) {
                    continue;
                }
                int distance = std::abs(enemy.x - x) + std::abs(enemy.y - y);
                if (distance <= 7) {
                    score += (8 - distance) * (enemy.hp / 100.0);
                }
            }

            int castleDistance = manhattanDistance(cell, castle);
            score += std::max(0, 8 - castleDistance) * 1.5;

            if (score > bestScore) {
                bestScore = score;
                bestTile = cell;
            }
        }
    }

    return bestTile;
}

} // namespace tower_siege
