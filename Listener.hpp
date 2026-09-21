#pragma once

#include "Types.hpp"

namespace pacman {

// Captures raw user input (keyboard) and translates it into game commands.
// The concrete input backend (console, SFML, SDL, etc.) can be plugged in
// by implementing these methods in a .cpp file.
class Listener {
public:
    Listener();

    // Polls the input backend for new events. Should be called once per frame.
    void poll();

    // Returns the most recently requested movement direction.
    Direction getRequestedDirection() const { return requestedDirection_; }

    // Returns true if the user requested to pause/resume the game this frame.
    bool wasPauseRequested() const { return pauseRequested_; }

    // Returns true if the user requested to quit the game.
    bool wasQuitRequested() const { return quitRequested_; }

private:
    Direction requestedDirection_ = Direction::None;
    bool pauseRequested_ = false;
    bool quitRequested_ = false;
};

} // namespace pacman