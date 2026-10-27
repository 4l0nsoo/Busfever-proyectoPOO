#pragma once
#include <string>

class Bus {
private:
    int id;//identificador del bus
    int capacity;//cantidad de personas
    int color;
    std::string direction;//direccion en la que avanza
    int x;//posicion en x
    int y;//posicion en y

public:
    Bus(int id, int capacity, int color, const std::string& direction, int x, int y);

    void mover();
    int getLength() const;   // se calcula a partir de capacity (16 -> 1, 24 -> 2, 40 -> 3)

    // getters triviales (no van en el diagrama)
    int getId() const { return id; }
    int getCapacity() const { return capacity; }
    int getColor() const { return color; }
    const std::string& getDirection() const { return direction; }
    int getX() const { return x; }
    int getY() const { return y; }
};
