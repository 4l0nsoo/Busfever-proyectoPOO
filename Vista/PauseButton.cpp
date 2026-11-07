#include "PauseButton.h"
#include "SceneLayout.h"

// Trae ALLEGRO_COLOR y al_map_rgb.
// Ej: el azul del botón.
#include <allegro5/allegro.h>

// Módulo de figuras: rectángulos redondeados del botón y de las barras.
// Ej: al_draw_filled_rounded_rectangle.
#include <allegro5/allegro_primitives.h>

// Espacio de nombres anónimo: medidas que solo usa este archivo.
// Ej: otro .cpp no puede cambiar BUTTON_SIZE.
namespace {

// Lado del botón cuadrado y redondeo de sus esquinas.
// Ej: 56 x 56 px con esquinas de 14 px.
const float BUTTON_SIZE = 56.0f;
const float BUTTON_RADIUS = 14.0f;

// Altura del "costado" oscuro que da relieve.
// Ej: 5 px de azul oscuro debajo del botón.
const float BUTTON_DEPTH = 5.0f;

// Distancia del centro del botón a los bordes de arriba y de la derecha de la ventana.
// Ej: 46 px -> el centro queda en (576 - 46, 46) = (530, 46).
const float MARGIN = 46.0f;

}

// Constructor: ubica el botón en la esquina de arriba a la derecha.
// Ej: centro en (530, 46).
PauseButton::PauseButton()
    : centerX(SceneLayout::WIDTH - MARGIN), centerY(MARGIN), hovered(false) {}

void PauseButton::setMousePosition(float x, float y) {
    // Se ilumina si el mouse está encima.
    // Ej: (530, 46) -> hovered = true.
    hovered = contains(x, y);
}

bool PauseButton::contains(float x, float y) const {
    // Mitad del lado del botón.
    // Ej: 56 / 2 = 28.
    const float half = BUTTON_SIZE / 2.0f;

    // El punto está dentro si cae en el cuadrado del botón.
    // Ej: x entre 502 y 558, y entre 18 y 74 + el relieve.
    return x >= centerX - half && x <= centerX + half &&
           y >= centerY - half && y <= centerY + half + BUTTON_DEPTH;
}

void PauseButton::update(double seconds) {
    // El botón es quieto: no hay nada que actualizar.
    // Ej: (void)seconds evita el aviso de "parámetro sin usar".
    (void)seconds;
}

void PauseButton::draw() const {
    // Mitad del lado del botón, para calcular sus esquinas desde el centro.
    // Ej: 28 px.
    const float half = BUTTON_SIZE / 2.0f;

    // Si el mouse está encima, la cara baja 2 px (como si se presionara).
    // Ej: 0 si no está encima.
    const float press = hovered ? 2.0f : 0.0f;

    // Cara del botón: azul, o un azul más claro si el mouse está encima.
    // Ej: al_map_rgb(rojo, verde, azul).
    const ALLEGRO_COLOR face = hovered ? al_map_rgb(92, 165, 250) : al_map_rgb(61, 142, 240);

    // Costado oscuro del botón (relieve), un poco más abajo.
    // Ej: se ve como el borde de una tecla.
    al_draw_filled_rounded_rectangle(centerX - half, centerY - half + BUTTON_DEPTH,
                                     centerX + half, centerY + half + BUTTON_DEPTH,
                                     BUTTON_RADIUS, BUTTON_RADIUS, al_map_rgb(31, 92, 184));

    // Cara azul del botón.
    // Ej: (x1, y1) arriba a la izquierda y (x2, y2) abajo a la derecha.
    al_draw_filled_rounded_rectangle(centerX - half, centerY - half + press,
                                     centerX + half, centerY + half + press,
                                     BUTTON_RADIUS, BUTTON_RADIUS, face);

    // Color blanco de las barras.
    // Ej: blanco sobre azul se distingue fácil.
    const ALLEGRO_COLOR white = al_map_rgb(255, 255, 255);

    // Barra izquierda del símbolo de pausa.
    // Ej: de x = 520 a 527 y de 24 px de alto.
    al_draw_filled_rounded_rectangle(centerX - 10.0f, centerY - 12.0f + press,
                                     centerX - 3.0f, centerY + 12.0f + press, 2.5f, 2.5f, white);

    // Barra derecha del símbolo de pausa.
    // Ej: de x = 533 a 540.
    al_draw_filled_rounded_rectangle(centerX + 3.0f, centerY - 12.0f + press,
                                     centerX + 10.0f, centerY + 12.0f + press, 2.5f, 2.5f, white);
}
