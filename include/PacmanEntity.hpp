#ifndef PACMAN_ENTITY_HPP
#define PACMAN_ENTITY_HPP

#include <iostream>
#include <string>
#include "IRenderable.hpp"
#include "Position.hpp"

class PacmanEntity : public IRenderable {
private:
    std::string name;
    Position pos;
    int score;

public:
    // 1. Constructor implicit
    PacmanEntity() : name("Default"), pos(0, 0), score(0) {}

    // 3. Constructor cu parametri
    PacmanEntity(const std::string& name, const Position& pos, int score)
        : name(name), pos(pos), score(score) {
    }

    // 2. Constructor de copiere
    PacmanEntity(const PacmanEntity& other)
        : name(other.name), pos(other.pos), score(other.score) {
    }

    // Destructor
    ~PacmanEntity() override = default;

    // 4. Operator de atribuire
    PacmanEntity& operator=(const PacmanEntity& other) {
        if (this != &other) {
            name = other.name;
            pos = other.pos;
            score = other.score;
        }
        return *this;
    }

    // 5. Operatori de comparare (comparăm după scor)
    bool operator==(const PacmanEntity& other) const {
        return (score == other.score && name == other.name && pos == other.pos);
    }

    bool operator<(const PacmanEntity& other) const {
        return score < other.score;
    }

    // 6. Operatori de intrare și ieșire
    friend std::ostream& operator<<(std::ostream& out, const PacmanEntity& e) {
        out << "Entitate: " << e.name << " | Poziție: " << e.pos << " | Scor: " << e.score;
        return out;
    }

    friend std::istream& operator>>(std::istream& in, PacmanEntity& e) {
        std::cout << "Introdu Nume, Poziție (X Y) și Scor: ";
        in >> e.name >> e.pos >> e.score;
        return in;
    }

    // Implementarea randării (Cerința 7)
    void render() const override {
        std::cout << "Se randează entitatea '" << name << "' la poziția " << pos << "\n";
    }
};

#endif