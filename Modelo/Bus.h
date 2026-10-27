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
    int seatsLeft;

public:
    Bus(int id, int capacity, int color, const std::string& direction, int x, int y);

    void mover();
    int getLength() const;   // se calcula a partir de capacity (16 -> 1, 24 -> 2, 40 -> 3)

    // getters triviales (no van en el diagrama)
    int board(int n);
    bool isFull() const;
    int getId() const; 
    int getCapacity() const;
    int getColor() const;
    std::string getDirection() const;
    int getX() const;
    int getY() const;
    int getSeatsLeft() const; //Disponibilidad de asientos
};

/*board(n): resta n asientos. Aquí puedes protegerte, por ejemplo no restar más de los que quedan.
isFull(): devuelve true si seatsLeft == 0.
getSeatsLeft(): getter, para que la sala sepa cuánto cupo hay (implícito, no se dibuja).*/