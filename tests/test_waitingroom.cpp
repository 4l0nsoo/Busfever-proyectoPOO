#include <iostream>
#include <cassert>
#include "../Modelo/WaitingRoom.h"
#include "tests.h"

void probarWaitingRoom() {
    Parking pk;
    Queue q;
    WaitingRoom w(pk, q);
    assert(w.bothRowsEmpty());

    // la sala recibe maximo 120 personas
    q.add(Passager(1, 100));
    q.add(Passager(2, 100));
    w.fillFromQueue();
    assert(w.getPeopleCount() == WaitingRoom::CAPACITY);
    assert(w.getSeats().size() == 2);
    assert(q.getPassengers()[0].getGroupAmount() == 80);
    assert(!w.bothRowsEmpty());

    // sin bus del mismo color nadie sube
    assert(!w.someoneCanBoard());
    Bus b1(1, 16, 1, "derecha", 0, 0);
    pk.receiveBus(b1);
    assert(w.someoneCanBoard());

    // el bus se llena y sale del parqueadero
    w.freePassager(pk);
    assert(pk.getBuses().size() == 0);
    assert(w.getPeopleCount() == 120);   // la sala se vuelve a llenar desde la fila

    w.clear();
    assert(w.getPeopleCount() == 0);

    std::cout << "WaitingRoom OK" << std::endl;
}
