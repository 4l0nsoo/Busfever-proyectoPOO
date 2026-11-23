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
    void clear();
    // Saca el primer grupo completo de la fila y lo retorna (ya no es const porque modifica la fila).
    // Ej: si la fila es [azul 8, rojo 16], retorna azul 8 y la fila queda [rojo 16].
    Passager removerFirstPassanger (); // Permite sacar el primer pasajero
    bool emptyFila()const; //dice si la fila tiene personas (retorna True) o si no tiene personas (retorna False)

    // Saca del frente hasta maxAmount personas; si el primer grupo es más grande, lo divide.
    // Ej: fila [azul 16], takeFront(10) retorna azul 10 y en la fila queda azul 6.
    Passager takeFront(int maxAmount);

    // Permite consultar los grupos de la fila sin modificarlos (para dibujarlos).
    // Ej: la Vista recorre getPassengers() para dibujar a las personas en los caminos.
    const std::vector<Passager>& getPassengers() const;
};
#endif // QUEUE_H