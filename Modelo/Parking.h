#pragma once
#include <vector>
#include "Bus.h"

class Parking {
private:
    std::vector<Bus> places;

public:
    Parking() = default;

    void ride();
};
