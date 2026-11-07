/**
 * @file Scenery.h
 * @brief Definición de la clase Scenery
 *
 * @details
 * Dibuja el fondo del juego con Allegro: la imagen assets/escenario.png con el
 * pasto, el camino de la fila, la glorieta, la franja de parqueo y la zona del
 * tablero. No conoce las reglas del juego: solo carga y dibuja la imagen.
 *
 * -----------------------------------------------------------------------------
 * Estándar CRC (Clase - Responsabilidad - Colaboración)
 * -----------------------------------------------------------------------------
 *
 * Clase:
 *   Scenery
 *
 * Responsabilidades:
 *  - Cargar la imagen de fondo desde un archivo.
 *  - Dibujar la imagen de fondo en la ventana.
 *  - Liberar la imagen al terminar.
 *
 * Colaboradores:
 *  - AllegroWindow:
 *      * Crea la ventana sobre la que Scenery dibuja el fondo.
 *
 * -----------------------------------------------------------------------------
 *
 * @author Santiago Caicedo Cabrera, Gabriela Castaño Castillo, Alonso Javier Diaz Veliz, Jesus Rodrigo Arango Arias, Luis Felipe Mosquera Machado
 * @email santiago.caicedo.cabrera@correounivalle.edu.co, gabriela.castano@correounivalle.edu.co, alonso.diaz@correounivalle.edu.co, arango.jesus@correounivalle.edu.co
 */

#pragma once

// Trae los tipos de Allegro, como ALLEGRO_BITMAP (una imagen en memoria).
// Ej: el fondo escenario.png se guarda en un ALLEGRO_BITMAP*.
#include <allegro5/allegro.h>

// Trae std::string para recibir la ruta del archivo.
// Ej: "assets/escenario.png".
#include <string>

class Scenery {
private:
    // Imagen de fondo (nullptr mientras no se cargue).
    // Ej: al_load_bitmap("assets/escenario.png") devuelve este puntero.
    ALLEGRO_BITMAP* background;

public:
    // Constructor: todavía no carga nada, deja el puntero en nullptr.
    // Ej: Scenery scenery; no abre ningún archivo aún.
    Scenery();

    // Destructor: libera la imagen de fondo.
    // Ej: se llama solo al terminar el main.
    ~Scenery();

    // Se prohíbe copiar: dos objetos liberarían la misma imagen.
    // Ej: Scenery otra = scenery; da error de compilación.
    Scenery(const Scenery&) = delete;

    // Se prohíbe asignar un fondo a otro por la misma razón.
    // Ej: otra = scenery; también da error de compilación.
    Scenery& operator=(const Scenery&) = delete;

    // Carga la imagen de fondo; retorna false si no la encuentra.
    // Ej: scenery.loadBackground("assets/escenario.png");
    // Debe llamarse DESPUÉS de crear la ventana (AllegroWindow::initialize).
    bool loadBackground(const std::string& backgroundPath);

    // Dibuja la imagen de fondo ocupando toda la ventana.
    // Ej: scenery.draw(); y luego window.present(); para verla.
    void draw() const;
};
