#include <iostream>
#include <cassert>
#include "../Modelo/Passager.h"
#include "tests.h"

void probarPassager() {
    Passager p(1, 10);
    assert(p.getColor() == 1);
    assert(p.getGroupAmount() == 10);

    // dividir una parte
    Passager g = p.split(4);
    assert(g.getGroupAmount() == 4);
    assert(p.getGroupAmount() == 6);

    // pedir mas de los que hay
    Passager h = p.split(20);
    assert(h.getGroupAmount() == 6);
    assert(p.getGroupAmount() == 0);

    // n invalido
    Passager q(2, 5);
    assert(q.split(0).getGroupAmount() == 0);
    assert(q.getGroupAmount() == 5);

    std::cout << "Passager OK" << std::endl;
}
