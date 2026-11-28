#include "Car.h"

Car::Car(int id, int color, const std::string& direction, int x, int y)
    : Vehicle(id, 16, color, direction, x, y) {}

int Car::getLength() const {
    return 1;
}