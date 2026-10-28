#include "WaitingRoom.h"

void WaitingRoom::freePassager(Parking& slots) {
    for(Passager& p: seats){
        Bus* bus = slots.findBus(p.getColor());

    }
}

void WaitingRoom::fillBus(Bus& bus) {
    size_t i = 0;
    while (i < seats.size() && !bus.isFull()) {
        Passager& p = seats[i];

        if (p.getColor() != bus.getColor()) {
            i++;                                  // otro color: siguiente grupo
            continue;
        }

        int antes = p.getGroupAmount();           // cuántos había
        int sobrantes = bus.board(antes);         // sube los que caben

        if (sobrantes == 0) {
            seats.erase(seats.begin() + i);       // subió completo: sale de la sala
        } else {
            p.split(antes - sobrantes);           // subieron esos: en p quedan los sobrantes
        }
    }
}