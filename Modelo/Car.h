#pragma once
#include "Vehicle.h"

class Car : public Vehicle {
public:
    // Un carro siempre tiene capacidad 16: no se pasa por parámetro.
    Car(int id, int color, const std::string& direction, int x, int y);

    int getLength() const override;
};