#include "Listener.hpp"

#include <iostream>
#include <string>

namespace pacman {

Listener::Listener() = default;

// Citeste o linie de la tastatura si o transforma in comanda.
// Enter fara text = nicio schimbare de directie.
void Listener::poll() {
    requestedDirection_ = Direction::None;
    pauseRequested_ = false;
    quitRequested_ = false;

    std::string line;
    if (!std::getline(std::cin, line)) {
        quitRequested_ = true; // intrarea s-a inchis (Ctrl+D / Ctrl+Z)
        return;
    }
    if (line.empty()) {
        return;
    }

    switch (line[0]) {
        case 'w': case 'W': requestedDirection_ = Direction::Up; break;
        case 's': case 'S': requestedDirection_ = Direction::Down; break;
        case 'a': case 'A': requestedDirection_ = Direction::Left; break;
        case 'd': case 'D': requestedDirection_ = Direction::Right; break;
        case 'p': case 'P': pauseRequested_ = true; break;
        case 'q': case 'Q': quitRequested_ = true; break;
        default: break;
    }
}

} // namespace pacman
