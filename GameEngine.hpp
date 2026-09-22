#pragma once

#include "Types.hpp"
#include <vector>

namespace pacman {

class Renderer;
class Listener;

class GameEngine {
public:
    GameEngine();

    void initialize();

    void update(float deltaTime);

    void handleInput(Direction requestedDirection);

    const Maze& getMaze() const { return maze_; }
    const Player& getPlayer() const { return player_; }
    const std::vector<Ghost>& getGhosts() const { return ghosts_; }
    GameState getState() const { return state_; }

    bool isRunning() const { return state_ == GameState::Playing || state_ == GameState::Paused; }

private:
    void moveEntity(Entity& entity, float deltaTime);
    void checkCollisions();
    void updateGhostModes(float deltaTime);
    void collectPellet(const Vector2i& position);
    bool isWalkable(const Vector2i& position) const;

    Maze maze_;
    Player player_;
    std::vector<Ghost> ghosts_;
    GameState state_ = GameState::Menu;

    float modeTimer_ = 0.0f;
};

} // namespace pacman
