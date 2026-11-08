/**
 * @file SceneryWindow.h
 * @brief Definición de la clase SceneryWindow
 *
 * @details
 * Abre la ventana gráfica de Allegro y la mantiene conectada con la consola:
 * la solución del juego es la de consola (el Controller procesa los IDs que se
 * escriben ahí) y la ventana REFLEJA esa misma partida con gráficos. Por eso
 * hay un solo modelo y un solo Controller para las dos.
 *
 * Pantallas de la ventana: el menú principal (elegir Nivel 1, 2 o 3, o Salir),
 * el juego (fondo, personas caminando, buses del tablero y buses estacionados)
 * y la pausa.
 *
 * Controles:
 *  - Menú: clic en un botón, teclas 1, 2, 3 en la ventana, o escribir 1, 2, 3 en la consola.
 *  - Juego: escribir en la consola el ID de un bus (como siempre); 'salir' vuelve al menú.
 *    Botón de pausa, ESC o P abren el menú de pausa.
 *  - Pausa: "Continuar" (o ESC / P) vuelve a la partida; "Retirarse" vuelve al menú principal.
 *
 * -----------------------------------------------------------------------------
 * Estándar CRC (Clase - Responsabilidad - Colaboración)
 * -----------------------------------------------------------------------------
 *
 * Clase:
 *   SceneryWindow
 *
 * Responsabilidades:
 *  - Abrir la ventana y cargar el fondo, los sprites y las fuentes.
 *  - Mostrar el menú y atender sus clics y teclas.
 *  - Pedir al Controller que cargue el nivel elegido (en la ventana o en la consola).
 *  - Leer los comandos de la consola sin detener la ventana y pasárselos al Controller.
 *  - Mostrar en la consola el tablero y los mensajes, igual que el juego de consola.
 *  - Cambiar entre las pantallas de menú, juego y pausa.
 *
 * Colaboradores:
 *  - AllegroWindow:
 *      * Inicia Allegro, crea la ventana y entrega los eventos.
 *  - Controller, LevelCatalog:
 *      * Cargan el nivel y procesan los comandos (la solución del juego).
 *  - Console:
 *      * Imprime la partida y lee los comandos escritos.
 *  - MainMenu, PauseMenu, PauseButton, Scenery, PassengerRenderer, BoardRenderer,
 *    ParkingRenderer (IRenderable):
 *      * Dibujan los menús, el fondo, las personas, el tablero y el parqueadero.
 *  - SpriteBank:
 *      * Carga los sprites que usan los renderers.
 *  - Traffic, Queue, WaitingRoom, Parking:
 *      * Estado del juego que se dibuja (solo lectura).
 *  - main (Vista/main.cpp):
 *      * Crea un SceneryWindow y llama a run().
 *
 * -----------------------------------------------------------------------------
 *
 * @author Santiago Caicedo Cabrera, Gabriela Castaño Castillo, Alonso Javier Diaz Veliz, Jesus Rodrigo Arango Arias, Luis Felipe Mosquera Machado
 * @email santiago.caicedo.cabrera@correounivalle.edu.co, gabriela.castano@correounivalle.edu.co, alonso.diaz@correounivalle.edu.co, arango.jesus@correounivalle.edu.co
 */

#pragma once

// Trae std::string para guardar el título y las rutas.
// Ej: "assets/escenario.png".
#include <string>

// El Controller carga los niveles y procesa los comandos (la lógica no está en la Vista).
// Ej: controller.processCommand("7");
#include "../Controlador/Controller.h"

// Ventana de Allegro (se usa en el método que carga el nivel).
// Ej: para cambiar el título de la ventana.
#include "AllegroWindow.h"

// Consola: donde se juega la solución; la ventana la refleja.
// Ej: console.printBoard(traffic) y console.tryReadCommand(comando).
#include "Console.h"

class SceneryWindow {
private:
    // Pantallas que puede mostrar la ventana.
    // Ej: empieza en Menu, pasa a Playing al elegir un nivel y a Paused al pausar.
    enum class Screen {
        Menu,
        Playing,
        Paused
    };

    // Controller que carga los niveles y procesa los comandos.
    // Ej: el mismo controller que crea el main.
    Controller& controller;

    // Tablero del modelo (solo lectura, para dibujar los buses).
    // Ej: traffic.getBuses().
    const Traffic& traffic;

    // Fila estática del modelo (solo lectura, para dibujar los caminos).
    // Ej: queue.getPassengers().
    const Queue& queue;

    // Fila rotatoria del modelo (solo lectura, para dibujar la glorieta).
    // Ej: waitingRoom.getSeats().
    const WaitingRoom& waitingRoom;

    // Parqueadero del modelo (solo lectura, para dibujar los buses estacionados).
    // Ej: parking.getBuses().
    const Parking& parking;

    // Consola conectada a la ventana: misma partida en texto y lugar donde se escriben los IDs.
    // Ej: console.printMessage("Bus enviado al estacionamiento.").
    Console& console;

    // Texto base de la barra de título de la ventana.
    // Ej: "Bus Fever Party! FPOO-C++ Edition".
    std::string title;

    // Ruta de la imagen de fondo, relativa a la carpeta del proyecto.
    // Ej: si mueves la imagen a otra carpeta, solo cambias esta ruta.
    std::string backgroundPath;

    // Ruta de la carpeta de los sprites.
    // Ej: "assets/sprites".
    std::string spritesPath;

    // Ruta de la fuente .ttf de los textos del menú.
    // Ej: "C:/Windows/Fonts/arialbd.ttf".
    std::string fontPath;

    // Pide al Controller cargar el nivel y actualiza el título; retorna false si falla.
    // Ej: loadLevel(window, Difficulty::Hard) carga nivel3_dificil.txt.
    bool loadLevel(AllegroWindow& window, Difficulty difficulty);

public:
    // Constructor: recibe el Controller, el modelo y la consola, y guarda las rutas.
    // Ej: SceneryWindow sceneryWindow(controller, traffic, queue, waitingRoom, parking, console);
    SceneryWindow(Controller& controller, const Traffic& traffic, const Queue& queue,
                  const WaitingRoom& waitingRoom, const Parking& parking, Console& console,
                  const std::string& title = "Bus Fever Party! FPOO-C++ Edition",
                  const std::string& backgroundPath = "assets/escenario.png",
                  const std::string& spritesPath = "assets/sprites",
                  const std::string& fontPath = "C:/Windows/Fonts/arialbd.ttf");

    // Abre la ventana con el menú y la mantiene conectada con la consola hasta cerrarla.
    // Ej: if (!sceneryWindow.run()) { ...jugar solo por consola... }
    // Retorna false si no se pudo abrir la ventana o faltan imágenes.
    bool run();
};
