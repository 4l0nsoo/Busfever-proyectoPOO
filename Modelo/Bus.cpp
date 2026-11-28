#include "Bus.h"

Bus::Bus(int id, int capacity, int color, const std::string& direction, int x, int y)
    : Vehicle(id, capacity, color, direction, x, y) {}

int Bus::getLength() const {
    if (getCapacity() == 16) { return 1; }
    if (getCapacity() == 24) { return 2; }
    if (getCapacity() == 40) { return 3; }
    return 0;
}