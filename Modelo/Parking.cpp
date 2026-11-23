#include "Parking.h"

void Parking::dispatch(int b) {
    places.erase(places.begin() + b);
}
Bus* Parking::findBus(int color) {
    for (Bus& b : places) {                       // & para no copiar el bus
        if (b.getColor() == color && !b.isFull()) {
            return &b;
        }
    }
    return nullptr;
}

bool Parking::isFull() {
    return places.size() >= 4;
}
void Parking::receiveBus(Bus &bus){
    places.push_back(bus);
}

const std::vector<Bus>& Parking::getBuses() const {
    // Retorna los buses estacionados por referencia constante: se pueden leer pero no cambiar.
    // Ej: la ventana de Allegro los dibuja en los puestos de parqueo.
    return places;
}

void Parking::clear() {
    // Saca todos los buses del parqueadero.
    // Ej: places pasa de [bus 2, bus 7] a [].
    places.clear();
}