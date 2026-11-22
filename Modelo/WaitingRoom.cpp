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

void WaitingRoom::clear() {
    // Borra todos los grupos de la fila rotatoria.
    // Ej: seats pasa de [rojo 16, azul 8] a [].
    seats.clear();
}

void WaitingRoom::fillFromQueue() {
    // Cupos libres = capacidad menos las personas que ya están.
    // Ej: 120 - 24 = 96 cupos libres.
    int freeSpace = CAPACITY - getPeopleCount();

    // Mientras quede cupo y haya personas en la fila estática, se sigue llenando.
    // Ej: se detiene al llegar a 120 personas o cuando la fila estática se vacía.
    while (freeSpace > 0 && !queue.emptyFila()) {
        // Saca del frente de la fila estática las personas que quepan.
        // Ej: con 10 cupos y primer grupo rojo 16, sale rojo 10 (quedan 6 en la fila).
        Passager group = queue.takeFront(freeSpace);

        // Agrega el grupo al final de la fila rotatoria.
        // Ej: seats pasa de [azul 8] a [azul 8, rojo 10].
        seats.push_back(group);

        // Descuenta los cupos que ocupó el grupo.
        // Ej: 10 cupos - 10 personas = 0 cupos libres.
        freeSpace -= group.getGroupAmount();
    }
}

int WaitingRoom::getPeopleCount() const {
    // Acumulador de personas.
    // Ej: empieza en 0.
    int total = 0;

    // Suma la cantidad de personas de cada grupo.
    // Ej: [rojo 16, azul 8] -> 16 + 8 = 24.
    for (const Passager& group : seats) {
        total += group.getGroupAmount();
    }

    // Retorna el total de personas.
    // Ej: 24.
    return total;
}

const std::vector<Passager>& WaitingRoom::getSeats() const {
    // Retorna los grupos por referencia constante: se pueden leer pero no cambiar.
    // Ej: la Vista los recorre para dibujarlos en la glorieta.
    return seats;
}