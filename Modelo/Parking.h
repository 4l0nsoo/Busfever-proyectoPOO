#pragma once
#include <vector>
#include "Bus.h"

class Parking {
private:
    std::vector<Bus> places;

public:
    Parking() = default;
    void receiveBus(Bus &bus);//recibe lso buses que salen de traffic
    Bus* findBus(int color);
    void dispatch(int b);//despacha los buses llenos
    bool isFull();

    // Permite consultar los buses estacionados sin modificarlos (para mostrarlos en la ventana).
    // Ej: la Vista dibuja el bus places[0] en el puesto 1 y places[1] en el puesto 2.
    const std::vector<Bus>& getBuses() const;

    // Vacía el parqueadero (se usa al cargar un nivel nuevo).
    // Ej: si en el nivel 1 se estacionaron 3 buses, el nivel 2 empieza con el parqueadero vacío.
    void clear();
};
