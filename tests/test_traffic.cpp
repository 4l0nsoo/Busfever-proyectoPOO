#include <iostream>
#include <cassert>
#include <stdexcept>
#include "../Modelo/Traffic.h"
#include "tests.h"

void probarTraffic() {
    Traffic t(6, 6);
    assert(t.getBusCount() == 0);

    // bus de largo 2 en (0,0) y (1,0)
    assert(t.addBus(Bus(1, 24, 1, "derecha", 0, 0)));
    assert(t.getGrid()[0][0] == 1);
    assert(t.getGrid()[0][1] == 1);
    assert(t.getBusCount() == 1);

    // casos donde no deberia dejarlo agregar
    assert(!t.addBus(Bus(2, 16, 2, "arriba", 1, 0)));    // choca con el bus 1
    assert(!t.addBus(Bus(1, 16, 2, "arriba", 3, 3)));    // id repetido
    assert(!t.addBus(Bus(3, 40, 1, "derecha", 4, 0)));   // se sale del tablero
    assert(!t.addBus(Bus(4, 16, 1, "diagonal", 3, 3)));  // direccion invalida
    assert(t.getBusCount() == 1);

    // salida libre
    assert(t.canExit(*t.getBus(1)));

    // otro bus tapando el camino
    assert(t.addBus(Bus(5, 16, 2, "arriba", 4, 0)));
    assert(!t.canExit(*t.getBus(1)));

    // al quitarlo vuelve a salir
    assert(t.removeBus(5));
    assert(t.getGrid()[0][4] == 0);
    assert(t.canExit(*t.getBus(1)));
    assert(!t.removeBus(99));

    assert(t.getBus(1) != nullptr);
    assert(t.getBus(99) == nullptr);

    // dimensiones invalidas lanzan excepcion
    bool lanzo = false;
    try {
        Traffic malo(0, 5);
    } catch (std::invalid_argument& e) {
        lanzo = true;
    }
    assert(lanzo);

    std::cout << "Traffic OK" << std::endl;
}
