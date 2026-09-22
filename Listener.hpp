#pragma once

#include "Types.hpp"

namespace pacman {

class Listener {
public:
    Listener();

    void poll();

    Direction getRequestedDirection() const { return requestedDirection_; }

    bool wasPauseRequested() const { return pauseRequested_; }

    bool wasQuitRequested() const { return quitRequested_; }

private:
    Direction requestedDirection_ = Direction::None;
    bool pauseRequested_ = false;
    bool quitRequested_ = false;
};

} 
