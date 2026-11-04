#include "Traffic.h"
#include <stdexcept>

// Constructor
Traffic::Traffic(int width, int height)
    : grid() {
    initializeBoard(width, height);
}

void Traffic::initializeBoard(int width, int height) {
    if (width <= 0 || height <= 0) {
        throw std::invalid_argument("Las dimensiones del tablero deben ser positivas");
    }

    grid.assign(height, std::vector<int>(width, 0));
    buses.clear();
}

// Traduce una dirección a pasos dentro de la cuadrícula.
bool Traffic::directionStep(const std::string& direction, int& dx, int& dy) {
    if (direction == "arriba")         { dx = 0;  dy = 1;  return true; }
    if (direction == "abajo")          { dx = 0;  dy = -1; return true; }
    if (direction == "derecha")        { dx = 1;  dy = 0;  return true; }
    if (direction == "izquierda")      { dx = -1; dy = 0;  return true; }
    return false;
}

// Retorna false cuando la posición está fuera de los límites del tablero.
bool Traffic::inBounds(int x, int y) const {
    return y >= 0 && y < static_cast<int>(grid.size()) &&
           x >= 0 && x < static_cast<int>(grid[y].size());
}
 
bool Traffic::addBus(const Bus& bus) {
    int dx, dy;
    if (bus.getId() <= 0 || !directionStep(bus.getDirection(), dx, dy)) return false;

    for (const Bus& current : buses) {
        if (current.getId() == bus.getId()) return false;
    }
 
    // Las coordenadas del nivel son la esquina inferior izquierda; las celdas crecen en x o y.
    int length = bus.getLength();
    if (length <= 0) return false;

    const int cellDx = dx == 0 ? 0 : 1;
    const int cellDy = dy == 0 ? 0 : 1;
    for (int i = 0; i < length; i++) {
        int cx = bus.getX() + cellDx * i;
        int cy = bus.getY() + cellDy * i;
        if (!inBounds(cx, cy) || grid[cy][cx] != 0) return false;
    }
    for (int i = 0; i < length; i++) {
        int cx = bus.getX() + cellDx * i;
        int cy = bus.getY() + cellDy * i;
        grid[cy][cx] = bus.getId();
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

    const int length = bus.getLength();
    if (length <= 0) return false;

    // Empieza fuera del extremo del bus que mira hacia su dirección de salida.
    int x = bus.getX();
    int y = bus.getY();
    if (dx > 0) x += length;
    if (dx < 0) x -= 1;
    if (dy > 0) y += length;
    if (dy < 0) y -= 1;

    while (inBounds(x, y)) {
        if (grid[y][x] != 0) return false;
        x += dx;
        y += dy;
    }
    return true;
}

Bus* Traffic::getBus(int id) {
    for (Bus& b : buses) {
        if (b.getId() == id) return &b;
    }
    return nullptr;
}

const std::vector<Bus>& Traffic::getBuses() const {
    return buses;
}
 
const std::vector<std::vector<int>>& Traffic::getGrid() const {
    return grid;
}
 
int Traffic::getBusCount() const {
    return static_cast<int>(buses.size());
}