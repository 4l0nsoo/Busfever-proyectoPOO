#pragma once
#include <vector>
#include "Passager.h"

class WaitingRoom {
private:
    std::vector<Passager> seats;

public:
    WaitingRoom() = default;

    void freePassager();
};
