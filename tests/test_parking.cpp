#include <iostream>
#include <cassert>
#include "../Modelo/Parking.h"
#include "tests.h"

void probarParking() {
    Parking p;
    Bus b(1, 16, 2, "derecha", 0, 0);
    p.receiveBus(b);
    assert(p.getBuses().size() == 1);

    // buscar por color
    assert(p.findBus(2) != nullptr);
    assert(p.findBus(3) == nullptr);

    // un bus lleno ya no lo encuentra y se despacha
    p.findBus(2)->board(16);
    assert(p.findBus(2) == nullptr);
    p.dispatchFullBuses();
    assert(p.getBuses().size() == 0);

    // el parqueadero se llena con 4 buses
    assert(!p.isFull());
    for (int i = 0; i < 4; i++) {
        Bus nuevo(i + 1, 16, 1, "arriba", 0, 0);
        p.receiveBus(nuevo);
    }
    assert(p.isFull());

    p.clear();
    assert(p.getBuses().size() == 0);

    std::cout << "Parking OK" << std::endl;
}
