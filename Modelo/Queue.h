#pragma once
#include <vector>
#include "Passager.h"

class Queue {
private:
    std::vector<Passager> queue;//vector estatico donde estan los pasajeros esperando entrar a WaitingRoom y despacha en forma FIFO

public:
    Queue() = default;

    void personQueue();
    void add(const Passager& passager);
};
