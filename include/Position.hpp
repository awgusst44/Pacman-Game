#ifndef POSITION_HPP
#define POSITION_HPP

#include <iostream>
#include "IRenderable.hpp"

class Position : public IRenderable {
private:
    int x;
    int y;

public:
    // 1. Constructor implicit
    Position() : x(0), y(0) {}

    // 3. Constructor cu parametri
    Position(int x, int y) : x(x), y(y) {}

    // 2. Constructor de copiere
    Position(const Position& other) : x(other.x), y(other.y) {}

    // Destructor
    ~Position() override = default;

    // 4. Operator de atribuire
    Position& operator=(const Position& other) {
        if (this != &other) {
            x = other.x;
            y = other.y;
        }
        return *this;
    }

    // Getteri și setteri
    int getX() const { return x; }
    int getY() const { return y; }
    void setX(int val) { x = val; }
    void setY(int val) { y = val; }

    // 5. Operatori de comparare
    bool operator==(const Position& other) const {
        return (x == other.x && y == other.y);
    }

    bool operator!=(const Position& other) const {
        return !(*this == other);
    }

    bool operator<(const Position& other) const {
        if (x != other.x) return x < other.x;
        return y < other.y;
    }

    // 6. Operatori de intrare și ieșire
    friend std::ostream& operator<<(std::ostream& out, const Position& p) {
        out << "(" << p.x << ", " << p.y << ")";
        return out;
    }

    friend std::istream& operator>>(std::istream& in, Position& p) {
        in >> p.x >> p.y;
        return in;
    }

    // Implementarea randării (Cerința 7)
    void render() const override {
        std::cout << "Poziție: (" << x << ", " << y << ")\n";
    }
};

#endif