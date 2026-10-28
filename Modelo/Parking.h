#pragma once
#include <vector>
#include "Bus.h"

class Parking {
private:
    std::vector<Bus> places;

public:
    Parking() = default;
    Bus* findBus(int color);
    void dispatch(int b);//despacha los buses llenos
    bool isFull();
};
