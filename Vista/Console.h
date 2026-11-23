/**
 * @file Console.h
 * @brief Definición de la clase Console 
 *
 * @details
 * Implementa la abstracción de la interfaz de usuario por consola. Se encarga de mostrar 
 * los estados del juego (tablero, mensajes, errores, victoria o derrota) y de leer 
 * los comandos que escribe el jugador. No contiene reglas del juego ni modifica el
 * modelo.
 *
 * -----------------------------------------------------------------------------
 * Estándar CRC (Clase - Responsabilidad - Colaboración)
 * -----------------------------------------------------------------------------
 *
 * Clase:
 *   Console 
 *
 * Responsabilidades:
 *  - Dibujar el tablero, identificando cada bus por su id.
 *  - Leer el comando escrito por el usuario (esperando, o sin detener la ventana de Allegro).
 *  - Mostrar mensajes, errores y el resultado final (ganar o perder).
 *
 * La consola es donde se juega la solución del juego; la ventana de Allegro
 * refleja esa misma partida con gráficos.
 *
 * Colaboradores:
 *   - Controller:
 *       * Le pide a Console mostrar la informacion y recibe el comando crudo.
 *   - Traffic:
 *       * Console utiliza Traffic para consultar el estado del tablero y dibujarlo.
 *   - SceneryWindow, ConsoleGame:
 *       * La usan para mostrar la partida y leer los comandos (con o sin ventana).
 * 
 *
 * -----------------------------------------------------------------------------
 *
 * @author Santiago Caicedo Cabrera, Gabriela Castaño Castillo, Alonso Javier Diaz Veliz, Jesus Rodrigo Arango Arias, Luis Felipe Mosquera Machado
 * @email santiago.caicedo.cabrera@correounivalle.edu.co, gabriela.castano@correounivalle.edu.co, alonso.diaz@correounivalle.edu.co, arango.jesus@correounivalle.edu.co
 *        

 */

#pragma once
#include <string>
#include "../Modelo/Traffic.h"

class Console {
private:
    // Lo que el usuario ha escrito y todavía no termina con Enter.
    // Ej: "1" mientras está escribiendo "12".
    std::string pendingInput;

public:
    void printBoard(const Traffic& traffic) const;//imprime el tablero despues que traffic lo haya convertido en matriz
    std::string readCommand() const;
    void printMessage(const std::string& message) const;//auxiliar para imprimir en consola
    void printError(const std::string& error) const;//imprime el mensaje de error
    void printWin() const;//imprime el mensaje de victoria
    void printLoss() const;//imprime el mensaje de derrota

    // Lee lo que se escribe SIN detener el programa; retorna true cuando se presiona Enter.
    // Ej: mientras la ventana de Allegro sigue animando, el usuario escribe "12" + Enter.
    // A diferencia de readCommand(), no se queda esperando: si no hay nada escrito, retorna false.
    bool tryReadCommand(std::string& command);
};
