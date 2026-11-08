/**
 * @file ConsoleGame.h
 * @brief Definición de la clase ConsoleGame
 *
 * @details
 * Es el juego por consola que antes estaba escrito dentro del main, con la misma
 * lógica: imprime el tablero, pide el ID de un bus, le pide al Controller que lo
 * mande al estacionamiento y gana cuando ya no quedan buses en el tablero. Ahora
 * también deja elegir el nivel (1, 2 o 3). Se usa cuando la ventana de Allegro
 * no se puede abrir, así el juego siempre se puede jugar.
 *
 * -----------------------------------------------------------------------------
 * Estándar CRC (Clase - Responsabilidad - Colaboración)
 * -----------------------------------------------------------------------------
 *
 * Clase:
 *   ConsoleGame
 *
 * Responsabilidades:
 *  - Pedir el nivel por consola y cargarlo con el Controller.
 *  - Repetir el turno: mostrar el tablero, leer un ID y enviarlo al Controller.
 *  - Mostrar si se ganó.
 *
 * Colaboradores:
 *  - Controller:
 *      * Carga el nivel y procesa los comandos (la solución del juego).
 *  - Console:
 *      * Imprime y lee en la consola.
 *  - Traffic:
 *      * Tablero que se muestra (solo lectura).
 *
 * -----------------------------------------------------------------------------
 *
 * @author Santiago Caicedo Cabrera, Gabriela Castaño Castillo, Alonso Javier Diaz Veliz, Jesus Rodrigo Arango Arias, Luis Felipe Mosquera Machado
 * @email santiago.caicedo.cabrera@correounivalle.edu.co, gabriela.castano@correounivalle.edu.co, alonso.diaz@correounivalle.edu.co, arango.jesus@correounivalle.edu.co
 */

#pragma once
#include "Console.h"
#include "../Controlador/Controller.h"

class ConsoleGame {
private:
    // Controller con la solución del juego (el mismo que usa la ventana).
    // Ej: controller.processCommand("7").
    Controller& controller;

    // Tablero del modelo (solo lectura, para imprimirlo).
    // Ej: console.printBoard(traffic).
    const Traffic& traffic;

    // Consola donde se imprime y se lee.
    // Ej: console.readCommand().
    Console& console;

    // Pide el nivel hasta que se cargue uno; retorna false si el usuario escribe "salir".
    // Ej: el usuario escribe "2" -> se carga el nivel normal.
    bool chooseLevel();

public:
    // Recibe el Controller, el tablero y la consola.
    // Ej: ConsoleGame consoleGame(controller, traffic, console);
    ConsoleGame(Controller& controller, const Traffic& traffic, Console& console);

    // Juega una partida completa por consola.
    // Ej: consoleGame.run(); termina al ganar o al escribir "salir".
    void run();
};
