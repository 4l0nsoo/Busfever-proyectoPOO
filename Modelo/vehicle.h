#pragma once
#include <string>

class Vehicle {
private:
    int id;
    int capacity;
    int color;
    std::string direction;
    int x;
    int y;
    int seatsLeft;

public:
    Vehicle(int id, int capacity, int color, const std::string& direction, int x, int y);
    virtual ~Vehicle() = default;

    // Cada tipo de vehículo responde distinto: por eso es virtual puro.
    virtual int getLength() const = 0;

    int board(int n);
    bool isFull() const;

    int getId() const;
    int getCapacity() const;
    int getColor() const;
    const std::string& getDirection() const;
    int getX() const;
    int getY() const;
    int getSeatsLeft() const;

    void setx(int x);
    void sety(int y);
    void setDirection(const std::string& direction);
};