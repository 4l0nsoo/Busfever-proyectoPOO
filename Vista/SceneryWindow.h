/**
 * @file SceneryWindow.h
 * @brief Definición de la clase SceneryWindow
 *
 * @details
 * Abre la ventana gráfica de Allegro con el fondo del juego y la mantiene
 * abierta hasta que el usuario la cierra (con la X o la tecla ESC). Reúne en un
 * solo método, run(), los pasos que antes estaban en mainAllegro.cpp, para que
 * el main del proyecto solo tenga que llamar a ese método.
 *
 * -----------------------------------------------------------------------------
 * Estándar CRC (Clase - Responsabilidad - Colaboración)
 * -----------------------------------------------------------------------------
 *
 * Clase:
 *   SceneryWindow
 *
 * Responsabilidades:
 *  - Guardar el título de la ventana y la ruta de la imagen de fondo.
 *  - Abrir la ventana de Allegro y cargar el fondo.
 *  - Mantener el ciclo de dibujo hasta que el usuario cierre la ventana.
 *  - Avisar si la ventana o el fondo no se pudieron cargar.
 *
 * Colaboradores:
 *  - AllegroWindow:
 *      * Inicia Allegro, crea la ventana y entrega los eventos.
 *  - Scenery:
 *      * Carga y dibuja la imagen de fondo.
 *  - main (Vista/main.cpp):
 *      * Crea un SceneryWindow y llama a run() para mostrar el escenario.
 *
 * -----------------------------------------------------------------------------
 *
 * @author Santiago Caicedo Cabrera, Gabriela Castaño Castillo, Alonso Javier Diaz Veliz, Jesus Rodrigo Arango Arias, Luis Felipe Mosquera Machado
 * @email santiago.caicedo.cabrera@correounivalle.edu.co, gabriela.castano@correounivalle.edu.co, alonso.diaz@correounivalle.edu.co, arango.jesus@correounivalle.edu.co
 */

#pragma once

// Trae std::string para guardar el título y la ruta del fondo.
// Ej: "assets/escenario.png".
#include <string>

class SceneryWindow {
private:
    // Texto que aparece en la barra de título de la ventana.
    // Ej: "Bus Fever Party! FPOO-C++ Edition".
    std::string title;

    // Ruta de la imagen de fondo, relativa a la carpeta del proyecto.
    // Ej: si mueves la imagen a otra carpeta, solo cambias esta ruta.
    std::string backgroundPath;

public:
    // Constructor: guarda el título y la ruta del fondo que se usarán en run().
    // Ej: SceneryWindow sceneryWindow; usa los valores por defecto.
    SceneryWindow(const std::string& title = "Bus Fever Party! FPOO-C++ Edition",
                  const std::string& backgroundPath = "assets/escenario.png");

    // Abre la ventana con el fondo y espera hasta que el usuario la cierre.
    // Ej: if (!sceneryWindow.run()) { ...mostrar error... }
    // Retorna false si no se pudo abrir la ventana o no se encontró el fondo.
    bool run();
};
