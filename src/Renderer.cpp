#include "Renderer.hpp"
#include "GameEngine.hpp"

#include <iostream>

namespace pacman {

namespace {
const int kHudRow = 11; // linia de sub harta unde se afiseaza scorul
}

Renderer::Renderer() = default;

Renderer::~Renderer() = default;

void Renderer::initialize() {
    std::cout << "\033[2J\033[H"; // sterge ecranul
}

// Deseneaza un cadru: harta, apoi jucatorul si fantomele peste ea, apoi HUD.
void Renderer::render(const GameEngine& engine) {
    std::cout << "\033[2J\033[H";
    drawMaze(engine.getMaze());
    drawPlayer(engine.getPlayer());
    drawGhosts(engine.getGhosts());
    drawHud(engine.getPlayer(), engine.getState());
    std::cout.flush();
}

void Renderer::shutdown() {
    std::cout << "\033[0m\n";
}

void Renderer::drawMaze(const Maze& maze) {
    for (int y = 0; y < maze.height; ++y) {
        for (int x = 0; x < maze.width; ++x) {
            switch (maze.at(x, y)) {
                case CellType::Wall:        std::cout << '#'; break;
                case CellType::Pellet:      std::cout << '.'; break;
                case CellType::PowerPellet: std::cout << 'O'; break;
                default:                    std::cout << ' '; break;
            }
        }
        std::cout << '\n';
    }
}

// Mutam cursorul la celula jucatorului (randurile/coloanele ANSI incep de la 1).
void Renderer::drawPlayer(const Player& player) {
    std::cout << "\033[" << player.position.y + 1 << ";" << player.position.x + 1 << "H"
              << (player.poweredUp ? '@' : 'C');
}

void Renderer::drawGhosts(const std::vector<Ghost>& ghosts) {
    for (const Ghost& ghost : ghosts) {
        char symbol = 'G';
        if (ghost.mode == GhostMode::Frightened) symbol = 'F';
        if (ghost.mode == GhostMode::Eaten)      symbol = 'x';
        std::cout << "\033[" << ghost.position.y + 1 << ";" << ghost.position.x + 1 << "H"
                  << symbol;
    }
}

void Renderer::drawHud(const Player& player, GameState state) {
    std::cout << "\033[" << kHudRow << ";1H"
              << "Scor: " << player.score << "   Vieti: " << player.lives << "\n";

    if (state == GameState::Won) {
        std::cout << "Ai castigat!\n";
    } else if (state == GameState::Lost) {
        std::cout << "Game over!\n";
    } else {
        std::cout << "w/a/s/d + Enter = miscare, Enter = continua, q = iesire\n";
    }
}

} // namespace pacman
