#include <iostream>
#include <cassert>
#include "../Modelo/Queue.h"
#include "tests.h"

void probarQueue() {
    Queue q;
    assert(q.emptyFila());

    q.add(Passager(1, 16));
    q.add(Passager(2, 8));
    assert(!q.emptyFila());
    assert(q.getPassengers().size() == 2);

    // el primer grupo no cabe completo, se divide
    Passager a = q.takeFront(10);
    assert(a.getColor() == 1);
    assert(a.getGroupAmount() == 10);
    assert(q.getPassengers()[0].getGroupAmount() == 6);

    // ahora si cabe completo
    Passager b = q.takeFront(100);
    assert(b.getGroupAmount() == 6);
    assert(q.getPassengers().size() == 1);

    // pedir 0 no cambia nada
    assert(q.takeFront(0).getGroupAmount() == 0);
    assert(q.getPassengers().size() == 1);

    Passager c = q.removerFirstPassanger();
    assert(c.getColor() == 2);
    assert(q.emptyFila());

    q.add(Passager(3, 5));
    q.clear();
    assert(q.emptyFila());

    std::cout << "Queue OK" << std::endl;
}
