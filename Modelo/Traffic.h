/**
 * @file Traffic.h
 * @brief Definición de la clase Traffic
 *
 * @details
 * Representa el tablero del juego. Se encarga de guardar los buses y sus
 * posiciones, y de decir si un bus puede salir. No muestra mensajes ni lee
 * comandos del jugador.
 *
 * -----------------------------------------------------------------------------
 * Estándar CRC (Clase - Responsabilidad - Colaboración)
 * -----------------------------------------------------------------------------
 *
 * Clase:
 *   Traffic
 *
 * Responsabilidades:
 *  - Guardar la cuadrícula del tráfico (grid), donde cada celda tiene el id del bus que la ocupa y 0 si está libre.
 *  - Leer el comando escrito por el usuario:
 *  - Mostrar mensajes, errores y el resultado final (ganar o perder).
 *  - Crear un tablero vacío de width x height celdas (constructor)
 *  - Agregar un bus al tablero y marcar sus celdas, rechazándolo si se sale del tablero o choca con otro (addBus)
 *  - Quitar un bus del nivel y liberar sus celdas (removeBus)

 * Colaboradores:
 *   - Bus:
 *      - Traffic usa Bus para guardadr los buses, leer sus datos y devolverlos.
 *
 * -----------------------------------------------------------------------------
 *
 * @author Santiago Caicedo Cabrera, Gabriela Castaño Castillo, Alonso Javier Diaz Veliz, Jesus Rodrigo Arango Cruz, Luis Felipe Mosquera Machado
 * @email santiago.caicedo.cabrera@correounivalle.edu.co, gabriela.castano@correounivalle.edu.co, alonso.diaz@correounivalle.edu.co, arango.jesus@correounivalle.edu.co
 * 
 */
#pragma once
#include <vector>
#include <string>
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
 
    //direcciones traducidas a pasos
    static bool directionStep(const std::string& direction, int& dx, int& dy);
 
    //retorna (True) si (x,y) está dentro del tablero
    bool inBounds(int x, int y) const;
 
public:
    Traffic() = default;
 
    //Crea un tablero vacío de width x height celdas
    Traffic(int width, int height);
 
    //FIFO, retorna (False) si el bus se sale del tablero o se choca con otro
    bool addBus(const Bus& bus);
 
    //quita la bus del nivel 
    bool removeBus(int id);
 
    //revisa si el camino está libre,
    bool canExit(const Bus& bus) const;
 
    //busca el bus por su identificación (id)
    Bus* getBus(int id);
 
    //Leer grid para que la consola lo imprima
    const std::vector<std::vector<int>>& getGrid() const;
 
    //función que muestra la cantidad de buses que siguen en el tablero
    int getBusCount() const;
};