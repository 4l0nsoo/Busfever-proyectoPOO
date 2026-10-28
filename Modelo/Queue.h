#pragma once
#include <vector>
#include "Passager.h"
#include <string>
#ifndef QUEUE_H
#define QUEUE_H
class Queue {
private:
    std::vector<Passager> queue;//vector estatico donde estan los pasajeros esperando entrar a WaitingRoom y despacha en forma FIFO

public:
    void personQueue();//movimiento de los pasajeros
    Queue() = default;//constructor por defecto
    void add(const Passager& passager);
    Passager removerFirstPassanger () const; // Permite sacar el primer pasajero 
    bool emptyFila()const; //dice si la fila tiene personas (retorna True) o si no tiene personas (retorna False)
};
#endif // QUEUE_H