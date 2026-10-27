#include "Bus.h"

Bus::Bus(int id, int capacity, int color, const std::string& direction, int x, int y)
    : id(id), capacity(capacity), color(color), direction(direction), x(x), y(y) {}

void Bus::mover() {
    // TODO
}

int Bus::getLength() const {
    // TODO: calcular el largo a partir de capacity
    return 0;
}
