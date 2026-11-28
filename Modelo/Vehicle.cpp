#include "Vehicle.h"

Vehicle::Vehicle(int id, int capacity, int color, const std::string& direction, int x, int y)
:id(id), capacity(capacity), color(color), direction(direction), x(x), y(y), seatsLeft(capacity) {}

// setters
void Vehicle::setx(int x){ this->x = x; }
void Vehicle::sety(int y){ this->y = y; }
void Vehicle::setDirection(const std::string& direction){ this->direction = direction; }

// getters
int Vehicle::getId() const { return id; }
int Vehicle::getCapacity() const { return capacity; }
int Vehicle::getColor() const { return color; }
const std::string& Vehicle::getDirection() const { return direction; }
int Vehicle::getX() const { return x; }
int Vehicle::getY() const { return y; }
int Vehicle::getSeatsLeft() const { return seatsLeft; }

// Verifica la disponibilidad de asientos
bool Vehicle::isFull() const {
    return seatsLeft == 0;
}

// Sube hasta n pasajeros y retorna los que quedaron por fuera
int Vehicle::board(int n){
    if(n<=seatsLeft){
        seatsLeft-=n;
        return 0;
    }
    int sobrantes= n-seatsLeft;
    seatsLeft=0;
    return sobrantes;
}