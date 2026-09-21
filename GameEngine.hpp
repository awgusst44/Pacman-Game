#pragma once

#include "Types.hpp"
#include <vector>

namespace pacman {

class Renderer;
class Listener;

// Central class that owns the game state and advances it over time.
class GameEngine {
public:
    GameEngine();

    // Loads/generates the maze and resets player/ghosts to their starting state.
    void initialize();

    // Advances the game logic by one time step (deltaTime in seconds).
    void update(float deltaTime);

    // Applies a direction change requested by the Listener.
    void handleInput(Direction requestedDirection);

    // Accessors used by the Renderer to draw the current state.
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