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

bool Parking::isFull(){
    if(places.size() >= 4){
        return true;
    };
}
