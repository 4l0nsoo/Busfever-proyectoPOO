#include "Parking.h"

void Parking::dispatch(int b) {
    places.erase(places.begin() + b);
}

bool Parking::isFull(){
    if(places.size() >= 4){
        return true;
    };
}
