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
 *  - Guardar la cuadrícula del tráfico; cada celda contiene el id del bus que la ocupa o 0 si está libre.
 *  - Crear o inicializar un tablero vacío de width x height celdas.
 *  - Agregar buses al vector del nivel y marcar sus celdas, rechazándolos si salen del tablero o chocan.
 *  - Quitar un bus del nivel y liberar sus celdas.
 *
 * Colaboradores:
 *   - Bus:
 *      - Traffic usa Bus para guardar los buses y consultar sus datos.
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
 
    // Traduce las direcciones a pasos dentro de la cuadrícula.
    static bool directionStep(const std::string& direction, int& dx, int& dy);
 
    // Retorna true si (x,y) está dentro del tablero.
    bool inBounds(int x, int y) const;
 
public:
    Traffic() = default;
 
    // Crea un tablero vacío de width x height celdas.
    Traffic(int width, int height);
 
    // Inicializa el tablero y limpia los buses cargados previamente.
    void initializeBoard(int width, int height);

    // Agrega el bus al vector y marca sus celdas; retorna false si no puede ubicarse.
    bool addBus(const Bus& bus);
 
    // Quita el bus del nivel y libera sus celdas.
    bool removeBus(int id);
 
    /**
     *  Revisa si un bus tiene el camino libre hasta el borde del tablero.
     *  El bus que se quiere sacar.
     *  true si todas las celdas del camino valen 0, false si encuentra otro bus.
     */
    bool canExit(const Bus& bus) const;
 
    // Busca el bus por su identificación (id).
    Bus* getBus(int id);

    // Permite consultar los buses para mostrar sus datos en consola.
    const std::vector<Bus>& getBuses() const;

    // Permite que la consola lea la cuadrícula.
    const std::vector<std::vector<int>>& getGrid() const;
 
    // Retorna la cantidad de buses que siguen en el tablero.
    int getBusCount() const;
};