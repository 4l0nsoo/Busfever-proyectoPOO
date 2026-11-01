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
};
