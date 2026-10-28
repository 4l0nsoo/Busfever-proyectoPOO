#include "Traffic.h"

//constructor
Traffic::Traffic(int width, int height)
    : grid(height, std::vector<int>(width, 0)) {}

//traducción de dirección a pasos
bool Traffic::directionStep(const std::string& direction, int& dx, int& dy) {
    if (direction == "arriba")         { dx = 0;  dy = 1;  return true; }
    if (direction == "abajo")          { dx = 0;  dy = -1; return true; }
    if (direction == "derecha")        { dx = 1;  dy = 0;  return true; }
    if (direction == "izquierda")      { dx = -1; dy = 0;  return true; }
    return false;
}

//cuando está fuera de los bordes retorna falso( no aparece explícitamente True ni False porque al haber una comparación con && podemos ver implícitamente su resultado)
bool Traffic::inBounds(int x, int y) const {
    return y >= 0 && y < static_cast<int>(grid.size()) &&
           x >= 0 && x < static_cast<int>(grid[y].size());
}
 
bool Traffic::addBus(const Bus& bus) {
    int dx, dy;
    if (!directionStep(bus.getDirection(), dx, dy)) return false;
 
    // (sentido contrario a la dirección).
    int length = bus.getLength();
    for (int i = 0; i < length; i++) {
        int cx = bus.getX() - dx * i;
        int cy = bus.getY() - dy * i;
        if (!inBounds(cx, cy) || grid[cy][cx] != 0) return false;
    }
    for (int i = 0; i < length; i++) {
        grid[bus.getY() - dy * i][bus.getX() - dx * i] = bus.getId();
    }
    buses.push_back(bus);
    return true;
}
 
bool Traffic::removeBus(int id) {
    for (size_t i = 0; i < buses.size(); i++) {
        if (buses[i].getId() == id) {
            // Liberar las celdas del grid que tenían este id.
            for (auto& row : grid)
                for (int& cell : row)
                    if (cell == id) cell = 0;
            buses.erase(buses.begin() + i);
            return true;
        }
    }
    return false;
}
 
bool Traffic::canExit(const Bus& bus) const {
    int dx, dy;
    if (!directionStep(bus.getDirection(), dx, dy)) return false;
    if (!inBounds(bus.getX(), bus.getY())) return false;
 
    // Recorre desde la celda siguiente a la cabeza hasta el borde del tablero.
    int x = bus.getX() + dx;
    int y = bus.getY() + dy;
    while (inBounds(x, y)) {
        if (grid[y][x] != 0) return false; //si otro bus bloquea el camino retorna falso
        x += dx;
        y += dy;
    }
    return true; //si no verdadero
}


Bus* Traffic::getBus(int id) {
    for (Bus& b : buses) {
        if (b.getId() == id) return &b;
    }
    return nullptr;
}
 
const std::vector<std::vector<int>>& Traffic::getGrid() const {
    return grid;
}
 
int Traffic::getBusCount() const {
    return static_cast<int>(buses.size());
}