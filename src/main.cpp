#include <iostream>
#include "Position.hpp"
#include "PacmanEntity.hpp"

int main() {
    // Test constructori și afișare (<<)
    Position p1(5, 10);
    PacmanEntity player("Pacman", p1, 100);

    std::cout << player << "\n";

    // Test randare (Cerința 7)
    player.render();

    // Test operator de atribuire și copiere
    Position p2;
    p2 = p1;

    if (p1 == p2) {
        std::cout << "Pozițiile sunt egale!\n";
    }

    return 0;
}
