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
 *  - Leer el comando escrito por el usuario:
 *  - Mostrar mensajes, errores y el resultado final (ganar o perder).
 *  
 *   
 * Colaboradores:
 *   - Controller:
 *       * Le pide a Console mostrar la informacion y recibe el comando crudo.
 *   - Traffic:
 *       * Console utiliza Traffic para consultar el estado del tablero y dibujarlo.
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
public:
    void printBoard(const Traffic& traffic) const;//imprime el tablero despues que traffic lo haya convertido en matriz
    std::string readCommand() const;
    void printMessage(const std::string& message) const;//auxiliar para imprimir en consola
    void printError(const std::string& error) const;//imprime el mensaje de error  
    void printWin() const;//imprime el mensaje de victoria
    void printLoss() const;//imprime el mensaje de derrota
};
