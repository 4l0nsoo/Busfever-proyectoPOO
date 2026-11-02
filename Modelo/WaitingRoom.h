#pragma once
#include <vector>
#include "Passager.h"
#include "Parking.h"
#include "Queue.h"


class WaitingRoom {
private:
    std::vector<Passager> seats;//el vector de la cantidad maxima de pasajeros a esperar por ser despachados
    Parking &parking;
    Queue &queue; 

public:
    WaitingRoom(Parking &p, Queue &q): parking(p), queue(q) {}; //como todos los metodos lo necesitan lo pasamos como construtor, si solo algunos los necesitaran lo pasaríamos como referencia a cada método
    void freePassager(Parking& slots);//despacha pasajeros a el bus del mismo color del pasajero
    void fillBus(Bus& bus);
};
