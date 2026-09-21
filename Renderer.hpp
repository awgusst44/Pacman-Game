#pragma once

#include "Types.hpp"
#include <vector>

namespace pacman {

class GameEngine;

// Responsible for drawing the maze, entities and HUD.
// The concrete rendering backend (console, SFML, SDL, etc.) can be
// plugged in by implementing these methods in a .cpp file.
class Renderer {
public:
    Renderer();
    ~Renderer();

    // Prepares any resources needed before the first frame (window, textures, etc.).
    void initialize();

    // Draws one full frame based on the current engine state.
    void render(const GameEngine& engine);

    // Releases any resources held by the renderer.
    void shutdown();

private:
    void drawMaze(const Maze& maze);
    void drawPlayer(const Player& player);
    void drawGhosts(const std::vector<Ghost>& ghosts);
    void drawHud(const Player& player, GameState state);

    int cellSize_ = 24; // pixels (or characters, for a console renderer)
};

} // namespace pacman