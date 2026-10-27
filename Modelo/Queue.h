#pragma once
#include <vector>
#include "Passager.h"

class Queue {
private:
    std::vector<Passager> queue;

public:
    Queue() = default;

    void personQueue();
};
