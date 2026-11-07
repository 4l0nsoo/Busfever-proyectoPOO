/**
 * @file SceneryWindow.h
 * @brief Definición de la clase SceneryWindow
 *
 * @details
 * Abre la ventana gráfica de Allegro y maneja sus dos pantallas: el menú
 * principal (elegir Nivel 1, 2 o 3, o Salir) y el juego (fondo, personas
 * caminando con colisiones y buses del tablero). Cuando el usuario elige un
 * nivel, le pide al Controller que lo cargue. No contiene reglas del juego:
 * las pide al Controller y solo lee el modelo para dibujarlo.
 *
 * Controles:
 *  - Menú: clic en un botón o teclas 1, 2, 3; ESC o "Salir" cierran la ventana.
 *  - Juego: botón de pausa (arriba a la derecha), ESC o P abren el menú de pausa.
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
 *  - Pedir al Controller que cargue el nivel elegido.
 *  - Cambiar entre las pantallas de menú, juego y pausa.
 *  - Congelar las animaciones mientras el juego está en pausa.
 *  - Mantener el ciclo de animación y dibujo hasta que el usuario cierre la ventana.
 *
 * Colaboradores:
 *  - AllegroWindow:
 *      * Inicia Allegro, crea la ventana y entrega los eventos.
 *  - Controller, LevelCatalog:
 *      * Cargan el nivel según la dificultad (lógica del juego).
 *  - MainMenu, PauseMenu, PauseButton, Scenery, PassengerRenderer, BoardRenderer (IRenderable):
 *      * Dibujan los menús, el botón de pausa, el fondo, las personas y los buses.
 *  - SpriteBank:
 *      * Carga los sprites que usan los renderers.
 *  - Traffic, Queue, WaitingRoom:
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

// El Controller carga los niveles (la lógica no está en la Vista).
// Ej: controller.loadLevel(Difficulty::Normal);
#include "../Controlador/Controller.h"

// Ventana de Allegro (se usa en el método que carga el nivel).
// Ej: para cambiar el título de la ventana.
#include "AllegroWindow.h"

class SceneryWindow {
private:
    // Pantallas que puede mostrar la ventana.
    // Ej: empieza en Menu, pasa a Playing al elegir un nivel y a Paused al pausar.
    enum class Screen {
        Menu,
        Playing,
        Paused
    };

    // Controller que carga los niveles.
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
    // Constructor: recibe el Controller y el modelo, y guarda las rutas.
    // Ej: SceneryWindow sceneryWindow(controller, traffic, queue, waitingRoom);
    SceneryWindow(Controller& controller, const Traffic& traffic, const Queue& queue,
                  const WaitingRoom& waitingRoom,
                  const std::string& title = "Bus Fever Party! FPOO-C++ Edition",
                  const std::string& backgroundPath = "assets/escenario.png",
                  const std::string& spritesPath = "assets/sprites",
                  const std::string& fontPath = "C:/Windows/Fonts/arialbd.ttf");

    // Abre la ventana con el menú; al elegir un nivel lo carga y lo anima hasta cerrar.
    // Ej: if (!sceneryWindow.run()) { ...mostrar error... }
    // Retorna false si no se pudo abrir la ventana o faltan imágenes.
    bool run();
};
