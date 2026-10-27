#pragma once
#include <vector>
#include "Passager.h"

class WaitingRoom {
private:
    std::vector<Passager> seats;//el vector de la cantidad maxima de pasajeros a esperar por ser despachados

public:
    WaitingRoom() = default;

    void freePassager();//despacha pasajeros a el bus del mismo color del pasajero
};
