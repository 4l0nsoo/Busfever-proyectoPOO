#pragma once
#include <vector>
#include "Passager.h"
#include "Parking.h"
#include "Queue.h"


class WaitingRoom {
private:
    std::vector<Passager> seats;//el vector de la cantidad maxima de pasajeros a esperar por ser despachados
    Parking &parking;
    Queue &queue; 

public:
    // Cantidad máxima de personas que caben en la fila rotatoria (la glorieta).
    // Ej: con 120, si la fila estática tiene 856 personas, solo las primeras 120 entran.
    static const int CAPACITY = 120;

    WaitingRoom(Parking &p, Queue &q): parking(p), queue(q) {}; //como todos los metodos lo necesitan lo pasamos como construtor, si solo algunos los necesitaran lo pasaríamos como referencia a cada método
    void freePassager(Parking& slots);//despacha pasajeros a el bus del mismo color del pasajero
    void fillBus(Bus& bus);

    // Vacía la fila rotatoria (se usa al cargar un nivel nuevo).
    // Ej: al pasar del nivel 1 al 2, la glorieta empieza vacía.
    void clear();

    // Pasa personas de la fila estática a la rotatoria hasta llenar CAPACITY.
    // Ej: si hay 100 personas y caben 120, pasan 20 más desde la fila estática.
    void fillFromQueue();

    // Retorna cuántas personas hay en total en la fila rotatoria.
    // Ej: dos grupos [rojo 16, azul 8] -> 24 personas.
    int getPeopleCount() const;

    // Permite consultar los grupos de la fila rotatoria sin modificarlos (para dibujarlos).
    // Ej: la Vista dibuja cada persona de cada grupo alrededor de la glorieta.
    const std::vector<Passager>& getSeats() const;
};
