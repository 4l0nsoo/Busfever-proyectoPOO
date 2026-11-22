#include "Scenery.h"

// Constructor: todavía no hay imagen cargada.
// Ej: el puntero en nullptr indica "todavía no existe".
Scenery::Scenery() : background(nullptr) {}

Scenery::~Scenery() {
    // Libera la imagen de fondo si se cargó.
    // Ej: cada al_load_bitmap necesita su al_destroy_bitmap.
    if (background != nullptr) {
        al_destroy_bitmap(background);
    }
}

bool Scenery::loadBackground(const std::string& backgroundPath) {
    // Carga la imagen de fondo desde el archivo.
    // Ej: "assets/escenario.png" -> imagen de 576 x 1024 en memoria.
    background = al_load_bitmap(backgroundPath.c_str());

    // Retorna true si se cargó y false si no se encontró el archivo.
    // Ej: falla si se borró la carpeta assets o se cambió el nombre de la imagen.
    return background != nullptr;
}

void Scenery::update(double seconds) {
    // El fondo es una imagen quieta: no hay nada que actualizar.
    // Ej: (void)seconds evita el aviso de "parámetro sin usar".
    (void)seconds;
}

void Scenery::draw() const {
    // Dibuja la imagen desde la esquina de arriba a la izquierda (0, 0).
    // Ej: el último 0 significa "sin voltear la imagen".
    al_draw_bitmap(background, 0, 0, 0);
}
