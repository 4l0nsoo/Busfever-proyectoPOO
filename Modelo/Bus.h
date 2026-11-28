#pragma once
#include "Vehicle.h"

class Bus : public Vehicle {
public:
    Bus(int id, int capacity, int color, const std::string& direction, int x, int y);

    // Sobrescribe el método virtual puro de Vehicle.
    int getLength() const override;
};