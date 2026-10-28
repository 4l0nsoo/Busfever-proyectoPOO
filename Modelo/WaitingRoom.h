#pragma once
#include <vector>
#include "Passager.h"
#include "Parking.h"


class WaitingRoom {
private:
    std::vector<Passager> seats;//el vector de la cantidad maxima de pasajeros a esperar por ser despachados

public:
    WaitingRoom() = default;

    void freePassager(Parking& slots);//despacha pasajeros a el bus del mismo color del pasajero
    void fillBus(Bus& bus);
};
