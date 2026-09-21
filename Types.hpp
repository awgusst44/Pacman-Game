#pragma once

#include <cstdint>
#include <vector>

namespace pacman {

// A discrete position on the maze grid.
struct Vector2i {
    int x = 0;
    int y = 0;

    bool operator==(const Vector2i& other) const {
        return x == other.x && y == other.y;
    }
    bool operator!=(const Vector2i& other) const {
        return !(*this == other);
    }
};

// Movement directions.
enum class Direction {
    None,
    Up,
    Down,
    Left,
    Right
};

// What a single maze cell contains.
enum class CellType {
    Wall,
    Empty,
    Pellet,
    PowerPellet,
    GhostHouse
};

// Overall state of the game session.
enum class GameState {
    Menu,
    Playing,
    Paused,
    Won,
    Lost
};

// Ghost AI behaviour mode.
enum class GhostMode {
    Chase,
    Scatter,
    Frightened,
    Eaten
};

// Base for anything that moves on the grid.
struct Entity {
    Vector2i position;
    Direction direction = Direction::None;
    float speed = 1.0f;
};

// Player-controlled character.
struct Player : public Entity {
    int score = 0;
    int lives = 3;
    bool poweredUp = false;
    float powerUpTimeRemaining = 0.0f;
};

// A single ghost.
struct Ghost : public Entity {
    GhostMode mode = GhostMode::Scatter;
    Vector2i homePosition;
    int id = 0; // used to distinguish ghost personalities (e.g. 0=Blinky, 1=Pinky, ...)
};

// The maze itself, stored as a 2D grid of cells.
struct Maze {
    int width = 0;
    int height = 0;
    std::vector<CellType> cells; // row-major: cells[y * width + x]

    CellType at(int x, int y) const {
        return cells.at(static_cast<size_t>(y) * width + x);
    }

    void set(int x, int y, CellType type) {
        cells.at(static_cast<size_t>(y) * width + x) = type;
    }
};

} // namespace pacman