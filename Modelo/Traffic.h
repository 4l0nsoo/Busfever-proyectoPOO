#pragma once
#include <vector>
#include "Bus.h"

/**
 * @brief Tablero de tráfico: guarda los buses del nivel y su posición en la cuadrícula.
 *
 * Decide si un bus puede salir (@ref canExit) y permite buscarlo por su id (@ref getBus).
 */
class Traffic {
private:
    /**
     * Matriz del tráfico, se accede como grid[y][x].
     * Cada celda guarda el ID del bus que la ocupa (los ids empiezan en 1).
     * 0 = celda libre.
     *
     * Ejes: y+1 = arriba, x+1 = derecha, y-1 = abajo, x-1 = izquierda.
     */
    std::vector<std::vector<int>> grid;

    // Los buses del nivel con todos sus datos (id, color, capacidad, dirección, posición).
    std::vector<Bus> buses;

public:
    Traffic() = default;

    /**
     *  Revisa si un bus tiene el camino libre hasta el borde del tablero.
     *  Bus que se quiere sacar.
     *  true si todas las celdas del camino valen 0, false si encuentra otro bus.
     */
    bool canExit(const Bus& bus) const;

    /**
     * Busca un bus por su id.
     * id Identificador del bus (el que ve el usuario en el tablero).
     * Puntero al bus, o nullptr si no existe.
     */
    Bus* getBus(int id);
};