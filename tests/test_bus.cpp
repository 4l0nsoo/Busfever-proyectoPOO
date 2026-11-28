#include <iostream>
#include <cassert>
#include "../Modelo/Bus.h"
#include "tests.h"

void probarBus() {
    Bus b(1, 16, 2, "derecha", 0, 0);
    assert(b.getId() == 1);
    assert(b.getColor() == 2);
    assert(b.getSeatsLeft() == 16);
    assert(!b.isFull());

    // largo segun la capacidad
    assert(b.getLength() == 1);
    assert(Bus(2, 24, 1, "arriba", 0, 0).getLength() == 2);
    assert(Bus(3, 40, 1, "arriba", 0, 0).getLength() == 3);

    // subir pasajeros
    assert(b.board(10) == 0);
    assert(b.getSeatsLeft() == 6);
    assert(b.board(10) == 4);   // sobran 4
    assert(b.isFull());

    std::cout << "Bus OK" << std::endl;
}
