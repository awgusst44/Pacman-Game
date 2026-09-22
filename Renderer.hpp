#pragma once

#include "Types.hpp"
#include <vector>

namespace pacman {

class GameEngine;

class Renderer {
public:
    Renderer();
    ~Renderer();

    void initialize();

    void render(const GameEngine& engine);

    void shutdown();

private:
    void drawMaze(const Maze& maze);
    void drawPlayer(const Player& player);
    void drawGhosts(const std::vector<Ghost>& ghosts);
    void drawHud(const Player& player, GameState state);

    int cellSize_ = 24; // pixels (or characters, for a console renderer)
};

} 
