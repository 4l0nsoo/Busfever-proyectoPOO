#pragma once
#include <vector>
#include "Bus.h"

class Traffic {
private:
    // grid[y][x]: cada celda guarda el color del bus (0 = libre)
    // y+1 = arriba, x+1 = derecha
    // y-1 = abajo,  x-1 = izquierda
    std::vector<std::vector<int>> grid;
    std::vector<Bus> buses;

public:
    Traffic() = default;

    bool canExit(const Bus& bus) const;
    Bus* getBus(int id);   // nullptr si no existe
};
