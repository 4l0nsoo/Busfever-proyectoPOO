#include "ParkingRenderer.h"
#include "SceneLayout.h"

// Trae std::min para escoger la escala más pequeña.
// Ej: std::min(0.5f, 1.0f) da 0.5.
#include <algorithm>

// Espacio de nombres anónimo: constantes que solo usa este archivo.
// Ej: otro .cpp no puede cambiar SPRITE_CELL.
namespace {

// Tamaño en píxeles de una celda dentro de los sprites de vehículos.
// Ej: un carro mide 64 x 64 y un bus grande 192 x 64 (3 celdas).
const float SPRITE_CELL = 64.0f;

// Largo y ancho máximos de un bus dentro de su puesto (el puesto mide 80 x 100).
// Ej: un bus grande se dibuja de 96 px de largo.
const float SLOT_MAX_LENGTH = 96.0f;
const float SLOT_MAX_WIDTH = 64.0f;

// Segundos de cada cuadro de la animación de los buses.
// Ej: 0.4.
const double VEHICLE_SECONDS_PER_FRAME = 0.4;

}

// Constructor: guarda las referencias y crea la animación de 2 cuadros.
// Ej: ParkingRenderer(parking, sprites).
ParkingRenderer::ParkingRenderer(const Parking& parking, const SpriteBank& sprites)
    : parking(parking), sprites(sprites),
      vehicleAnimation(SpriteBank::VEHICLE_FRAMES, VEHICLE_SECONDS_PER_FRAME) {}

void ParkingRenderer::update(double seconds) {
    // Avanza la animación de los buses estacionados.
    // Ej: cada 0.4 segundos cambia de cuadro.
    vehicleAnimation.update(seconds);
}

void ParkingRenderer::draw() const {
    // Buses del parqueadero en el orden en que llegaron.
    // Ej: [bus 2, bus 7] -> el bus 2 en el puesto 1 y el bus 7 en el puesto 2.
    const std::vector<Bus>& buses = parking.getBuses();

    // Recorre los buses, sin pasarse de los 4 puestos que hay en el dibujo.
    // Ej: i = 0 es el puesto 1.
    for (std::size_t i = 0; i < buses.size() && i < static_cast<std::size_t>(SceneLayout::PARKING_SLOTS); i++) {
        // Bus de este puesto.
        // Ej: bus rojo grande.
        const Bus& bus = buses[i];

        // Sprite del color, tamaño y cuadro de animación del bus.
        // Ej: busgrande_1.png rojo.
        const int frame = (vehicleAnimation.getFrame() + bus.getId()) % SpriteBank::VEHICLE_FRAMES;
        ALLEGRO_BITMAP* sprite = sprites.getVehicle(bus.getColor(), bus.getLength(), frame);
        if (sprite == nullptr) {
            continue;
        }

        // Centro del puesto.
        // Ej: puesto 1 -> (83 + 40, 360 + 50) = (123, 410).
        const float centerX = SceneLayout::SLOT_X[i] + SceneLayout::SLOT_WIDTH / 2.0f;
        const float centerY = SceneLayout::SLOT_Y + SceneLayout::SLOT_HEIGHT / 2.0f;

        // Escala para que el bus quepa en el puesto (a lo largo y a lo ancho).
        // Ej: bus grande (3 celdas) -> min(96 / 192, 64 / 64) = 0.5.
        const float scale = std::min(SLOT_MAX_LENGTH / (SPRITE_CELL * bus.getLength()),
                                     SLOT_MAX_WIDTH / SPRITE_CELL);

        // Dibuja el bus centrado en el puesto, mirando hacia arriba (-90 grados).
        // Ej: al_draw_scaled_rotated_bitmap(imagen, centroImgX, centroImgY, x, y, escalaX, escalaY, angulo, 0).
        al_draw_scaled_rotated_bitmap(sprite, al_get_bitmap_width(sprite) / 2.0f,
                                      al_get_bitmap_height(sprite) / 2.0f, centerX, centerY,
                                      scale, scale, -ALLEGRO_PI / 2.0f, 0);
    }
}
