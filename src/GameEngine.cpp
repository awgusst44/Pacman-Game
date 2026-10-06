#include "GameEngine.hpp"

#include <climits>
#include <cstdlib>
#include <cstring>

namespace pacman {

namespace {

// Harta jocului: # perete, . bulina, o bulina mare, G casa fantomelor, P start jucator
const char* const kLayout[] = {
    "###############",
    "#o.....#.....o#",
    "#.###.###.###.#",
    "#.............#",
    "#.###.#G#.###.#",
    "#.....GGG.....#",
    "#.###.###.###.#",
    "#o.....P.....o#",
    "###############",
};
const int kLayoutHeight = 9;

// Colturile spre care se indreapta fantomele in modul Scatter (dupa id).
const Vector2i kCorners[4] = {{13, 1}, {1, 1}, {13, 7}, {1, 7}};

const float kScatterChaseSeconds = 10.0f;
const float kPowerUpSeconds = 8.0f;

Vector2i toOffset(Direction d) {
    switch (d) {
        case Direction::Up:    return {0, -1};
        case Direction::Down:  return {0, 1};
        case Direction::Left:  return {-1, 0};
        case Direction::Right: return {1, 0};
        default:               return {0, 0};
    }
}

Direction opposite(Direction d) {
    switch (d) {
        case Direction::Up:    return Direction::Down;
        case Direction::Down:  return Direction::Up;
        case Direction::Left:  return Direction::Right;
        case Direction::Right: return Direction::Left;
        default:               return Direction::None;
    }
}

Vector2i add(const Vector2i& a, const Vector2i& b) {
    return {a.x + b.x, a.y + b.y};
}

int distanceSquared(const Vector2i& a, const Vector2i& b) {
    int dx = a.x - b.x;
    int dy = a.y - b.y;
    return dx * dx + dy * dy;
}

// Cauta pozitia 'P' in harta (pozitia de start a jucatorului).
Vector2i findPlayerStart() {
    for (int y = 0; y < kLayoutHeight; ++y) {
        for (int x = 0; kLayout[y][x] != '\0'; ++x) {
            if (kLayout[y][x] == 'P') {
                return {x, y};
            }
        }
    }
    return {1, 1};
}

int countPellets(const Maze& maze) {
    int count = 0;
    for (CellType c : maze.cells) {
        if (c == CellType::Pellet || c == CellType::PowerPellet) {
            ++count;
        }
    }
    return count;
}

} // namespace

GameEngine::GameEngine() = default;

// Construieste harta din kLayout si pune jucatorul si fantomele la start.
void GameEngine::initialize() {
    maze_ = Maze();
    maze_.height = kLayoutHeight;
    maze_.width = static_cast<int>(std::strlen(kLayout[0]));
    maze_.cells.assign(static_cast<size_t>(maze_.width) * maze_.height, CellType::Empty);

    player_ = Player();
    ghosts_.clear();

    for (int y = 0; y < maze_.height; ++y) {
        for (int x = 0; x < maze_.width; ++x) {
            switch (kLayout[y][x]) {
                case '#': maze_.set(x, y, CellType::Wall); break;
                case '.': maze_.set(x, y, CellType::Pellet); break;
                case 'o': maze_.set(x, y, CellType::PowerPellet); break;
                case 'G': {
                    maze_.set(x, y, CellType::GhostHouse);
                    Ghost ghost;
                    ghost.position = {x, y};
                    ghost.homePosition = {x, y};
                    ghost.id = static_cast<int>(ghosts_.size());
                    ghosts_.push_back(ghost);
                    break;
                }
                case 'P':
                    maze_.set(x, y, CellType::Empty);
                    player_.position = {x, y};
                    break;
                default: maze_.set(x, y, CellType::Empty); break;
            }
        }
    }

    modeTimer_ = 0.0f;
    state_ = GameState::Playing;
}

// Un "tick" de joc: deltaTime = secunde simulate in acest pas.
void GameEngine::update(float deltaTime) {
    if (state_ != GameState::Playing) {
        return;
    }

    // Timerul bonusului de la bulina mare.
    if (player_.poweredUp) {
        player_.powerUpTimeRemaining -= deltaTime;
        if (player_.powerUpTimeRemaining <= 0.0f) {
            player_.poweredUp = false;
            player_.powerUpTimeRemaining = 0.0f;
            for (Ghost& ghost : ghosts_) {
                if (ghost.mode == GhostMode::Frightened) {
                    ghost.mode = GhostMode::Chase;
                }
            }
        }
    }

    moveEntity(player_, deltaTime);
    collectPellet(player_.position);
    checkCollisions();
    if (state_ != GameState::Playing) {
        return;
    }

    // Alege directia fiecarei fantome (spre tinta, in functie de mod), apoi o muta.
    auto steer = [this](Ghost& g) {
        if (g.mode == GhostMode::Eaten && g.position == g.homePosition) {
            g.mode = GhostMode::Chase;
        }

        Vector2i target = player_.position;
        if (g.mode == GhostMode::Scatter) {
            target = kCorners[g.id % 4];
        } else if (g.mode == GhostMode::Eaten) {
            target = g.homePosition;
        }

        const Direction options[] = {Direction::Up, Direction::Left,
                                     Direction::Down, Direction::Right};
        Direction best = Direction::None;
        int bestScore = INT_MAX;

        for (Direction d : options) {
            if (g.direction != Direction::None && d == opposite(g.direction)) {
                continue; // fantomele nu se intorc din mers
            }
            Vector2i next = add(g.position, toOffset(d));
            if (!isWalkable(next)) {
                continue;
            }
            int score = (g.mode == GhostMode::Frightened)
                            ? std::rand() % 1000
                            : distanceSquared(next, target);
            if (score < bestScore) {
                bestScore = score;
                best = d;
            }
        }

        if (best == Direction::None) {
            best = opposite(g.direction); // fundatura: se intoarce
        }
        g.direction = best;
    };

    for (Ghost& ghost : ghosts_) {
        steer(ghost);
        moveEntity(ghost, deltaTime);
    }

    checkCollisions();
    updateGhostModes(deltaTime);
}

// Schimba directia jucatorului daca celula din directia ceruta este libera.
void GameEngine::handleInput(Direction requestedDirection) {
    if (state_ != GameState::Playing || requestedDirection == Direction::None) {
        return;
    }
    Vector2i next = add(player_.position, toOffset(requestedDirection));
    if (isWalkable(next)) {
        player_.direction = requestedDirection;
    }
}

// Muta entitatea cu o celula in directia ei, daca nu e perete.
void GameEngine::moveEntity(Entity& entity, float /*deltaTime*/) {
    Vector2i next = add(entity.position, toOffset(entity.direction));
    if (isWalkable(next)) {
        entity.position = next;
    }
}

// Pacman vs fantome: fantoma speriata e mancata, altfel jucatorul pierde o viata.
void GameEngine::checkCollisions() {
    for (Ghost& ghost : ghosts_) {
        if (ghost.position != player_.position || ghost.mode == GhostMode::Eaten) {
            continue;
        }

        if (ghost.mode == GhostMode::Frightened) {
            player_.score += 200;
            ghost.mode = GhostMode::Eaten;
            continue;
        }

        // Jucatorul a fost prins.
        player_.lives--;
        if (player_.lives <= 0) {
            state_ = GameState::Lost;
            return;
        }

        player_.position = findPlayerStart();
        player_.direction = Direction::None;
        player_.poweredUp = false;
        player_.powerUpTimeRemaining = 0.0f;
        for (Ghost& g : ghosts_) {
            g.position = g.homePosition;
            g.direction = Direction::None;
            g.mode = GhostMode::Scatter;
        }
        modeTimer_ = 0.0f;
        return;
    }
}

// Alterneaza fantomele intre Scatter si Chase la un interval fix.
void GameEngine::updateGhostModes(float deltaTime) {
    modeTimer_ += deltaTime;
    if (modeTimer_ < kScatterChaseSeconds) {
        return;
    }
    modeTimer_ = 0.0f;

    for (Ghost& ghost : ghosts_) {
        if (ghost.mode == GhostMode::Chase) {
            ghost.mode = GhostMode::Scatter;
        } else if (ghost.mode == GhostMode::Scatter) {
            ghost.mode = GhostMode::Chase;
        }
    }
}

// Mananca bulina de pe pozitia data; actualizeaza scorul, bonusul si victoria.
void GameEngine::collectPellet(const Vector2i& position) {
    CellType cell = maze_.at(position.x, position.y);

    if (cell == CellType::Pellet) {
        maze_.set(position.x, position.y, CellType::Empty);
        player_.score += 10;
    } else if (cell == CellType::PowerPellet) {
        maze_.set(position.x, position.y, CellType::Empty);
        player_.score += 50;
        player_.poweredUp = true;
        player_.powerUpTimeRemaining = kPowerUpSeconds;
        for (Ghost& ghost : ghosts_) {
            if (ghost.mode != GhostMode::Eaten) {
                ghost.mode = GhostMode::Frightened;
            }
        }
    } else {
        return;
    }

    if (countPellets(maze_) == 0) {
        state_ = GameState::Won;
    }
}

// Celula e libera daca e in harta si nu e perete.
bool GameEngine::isWalkable(const Vector2i& position) const {
    if (position.x < 0 || position.y < 0 ||
        position.x >= maze_.width || position.y >= maze_.height) {
        return false;
    }
    return maze_.at(position.x, position.y) != CellType::Wall;
}

} // namespace pacman
