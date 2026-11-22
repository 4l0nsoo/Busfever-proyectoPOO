#include "BoardRenderer.h"
#include "SceneLayout.h"

// Módulo de figuras: rectángulos redondeados para las celdas.
// Ej: al_draw_filled_rounded_rectangle.
#include <allegro5/allegro_primitives.h>

// Trae std::min para escoger el menor de dos números.
// Ej: std::min(64.0f, 42.0f) da 42.
#include <algorithm>

// Margen interno entre el borde del panel y el tablero.
// Ej: 14 px de aire alrededor de las celdas.
const float BOARD_PADDING = 14.0f;

// Tamaño en píxeles de una celda dentro de los sprites de vehículos.
// Ej: un carro mide 64 x 64 y un bus grande 192 x 64 (3 celdas).
const float SPRITE_CELL = 64.0f;

// Porcentaje de la celda que ocupa el vehículo (para dejar un espacio entre buses).
// Ej: 0.96 -> en una celda de 50 px el carro mide 48 px.
const float VEHICLE_FILL = 0.96f;

// Segundos que dura cada cuadro de la animación de los buses.
// Ej: 0.4 -> el bus cambia de cuadro 2 o 3 veces por segundo.
const double VEHICLE_SECONDS_PER_FRAME = 0.4;

// Constructor: guarda las referencias y crea la animación de 2 cuadros.
// Ej: los buses arrancan en el cuadro 0.
BoardRenderer::BoardRenderer(const Traffic& traffic, const SpriteBank& sprites)
    : traffic(traffic), sprites(sprites),
      vehicleAnimation(SpriteBank::VEHICLE_FRAMES, VEHICLE_SECONDS_PER_FRAME) {}

void BoardRenderer::update(double seconds) {
    // Avanza la animación de los buses con el tiempo que pasó.
    // Ej: cada 0.4 segundos alterna entre el cuadro 0 y el 1.
    vehicleAnimation.update(seconds);
}

float BoardRenderer::computeCellSize() const {
    // Cuadrícula del modelo: grid[y][x].
    // Ej: en el nivel fácil tiene 8 filas de 8 columnas.
    const std::vector<std::vector<int>>& grid = traffic.getGrid();

    // Si no hay tablero cargado, no hay celdas.
    // Ej: antes de cargar el primer nivel.
    if (grid.empty() || grid.front().empty()) {
        return 0.0f;
    }

    // Cantidad de columnas (ancho) y filas (alto) del tablero.
    // Ej: 12 y 12 en el nivel normal.
    const float columns = static_cast<float>(grid.front().size());
    const float rows = static_cast<float>(grid.size());

    // Espacio disponible dentro del panel, quitando el margen de los dos lados.
    // Ej: 536 - 2 * 14 = 508 de ancho.
    const float availableWidth = SceneLayout::BOARD_WIDTH - 2.0f * BOARD_PADDING;
    const float availableHeight = SceneLayout::BOARD_HEIGHT - 2.0f * BOARD_PADDING;

    // La celda es cuadrada: se usa el menor de los dos tamaños para que quepa.
    // Ej: min(508 / 8, 480 / 8) = min(63.5, 60) = 60 px.
    return std::min(availableWidth / columns, availableHeight / rows);
}

float BoardRenderer::directionAngle(const std::string& direction) {
    // Los sprites miran a la derecha, así que "derecha" no se gira.
    // Ej: un bus "derecha" se dibuja tal cual.
    if (direction == "derecha") return 0.0f;

    // "abajo": un cuarto de vuelta en el sentido del reloj.
    // Ej: ALLEGRO_PI / 2 = 90 grados.
    if (direction == "abajo") return ALLEGRO_PI / 2.0f;

    // "izquierda": media vuelta.
    // Ej: ALLEGRO_PI = 180 grados.
    if (direction == "izquierda") return ALLEGRO_PI;

    // "arriba": un cuarto de vuelta en contra del reloj.
    // Ej: -ALLEGRO_PI / 2 = -90 grados.
    if (direction == "arriba") return -ALLEGRO_PI / 2.0f;

    // Dirección desconocida: no se gira.
    // Ej: "diagonal" no existe en el juego.
    return 0.0f;
}

void BoardRenderer::draw() const {
    // Tamaño de cada celda en píxeles.
    // Ej: 60 px en el nivel fácil.
    const float cell = computeCellSize();

    // Si no hay tablero, no se dibuja nada.
    // Ej: si el nivel no se pudo cargar.
    if (cell <= 0.0f) {
        return;
    }

    // Cuadrícula del modelo y sus medidas.
    // Ej: 8 columnas x 8 filas.
    const std::vector<std::vector<int>>& grid = traffic.getGrid();
    const int columns = static_cast<int>(grid.front().size());
    const int rows = static_cast<int>(grid.size());

    // Esquina de arriba a la izquierda del tablero, centrado dentro del panel.
    // Ej: el panel mide 536 de ancho y el tablero 480 -> sobran 56, se dejan 28 a cada lado.
    const float originX = SceneLayout::BOARD_X + (SceneLayout::BOARD_WIDTH - columns * cell) / 2.0f;
    const float originY = SceneLayout::BOARD_Y + (SceneLayout::BOARD_HEIGHT - rows * cell) / 2.0f;

    // Color de las celdas vacías: un lila un poco más oscuro que el panel.
    // Ej: al_map_rgb(rojo, verde, azul).
    const ALLEGRO_COLOR cellColor = al_map_rgb(222, 226, 243);

    // Dibuja todas las celdas de la cuadrícula.
    // Ej: 8 x 8 = 64 celdas en el nivel fácil.
    for (int row = 0; row < rows; row++) {
        for (int column = 0; column < columns; column++) {
            // Esquina de la celda en pantalla.
            // Ej: la celda (columna 2, fila 1) está 2 celdas a la derecha y 1 abajo del origen.
            const float x = originX + column * cell;
            const float y = originY + row * cell;

            // Dibuja la celda con 2 px de separación y esquinas redondeadas.
            // Ej: se ven como baldosas sobre el panel.
            al_draw_filled_rounded_rectangle(x + 2, y + 2, x + cell - 2, y + cell - 2,
                                             cell * 0.15f, cell * 0.15f, cellColor);
        }
    }

    // Cuadro actual de la animación de los buses.
    // Ej: 0 o 1.
    const int frame = vehicleAnimation.getFrame();

    // Dibuja cada bus del tablero.
    // Ej: 34 buses en el nivel fácil.
    for (const Bus& bus : traffic.getBuses()) {
        // Largo del bus en celdas.
        // Ej: 1 carro, 2 bus mediano, 3 bus grande.
        const int length = bus.getLength();

        // Dirección hacia la que sale el bus.
        // Ej: "izquierda".
        const std::string direction = bus.getDirection();

        // El bus es horizontal si sale a la izquierda o a la derecha.
        // Ej: "arriba" y "abajo" son verticales.
        const bool horizontal = (direction == "izquierda" || direction == "derecha");

        // Columna de la celda izquierda del bus.
        // Ej: bus en x = 3 -> columna 3.
        const int column = bus.getX();

        // Fila de pantalla de la celda de arriba del bus (en el modelo y crece hacia arriba).
        // Ej: tablero de 8 filas, bus horizontal en y = 0 -> fila de pantalla 7 (la de abajo).
        const int topRow = horizontal ? (rows - 1 - bus.getY())
                                      : (rows - 1 - (bus.getY() + length - 1));

        // Ancho y alto del rectángulo que ocupa el bus, en píxeles.
        // Ej: bus grande horizontal en celdas de 60 -> 180 x 60.
        const float width = (horizontal ? length : 1) * cell;
        const float height = (horizontal ? 1 : length) * cell;

        // Centro de ese rectángulo, donde se dibuja el sprite.
        // Ej: el sprite se gira alrededor de este punto.
        const float centerX = originX + column * cell + width / 2.0f;
        const float centerY = originY + topRow * cell + height / 2.0f;

        // Desfasa la animación según el id para que no todos se muevan igual.
        // Ej: el bus 1 está en el cuadro 1 cuando el bus 2 está en el cuadro 0.
        const int busFrame = (frame + bus.getId()) % SpriteBank::VEHICLE_FRAMES;

        // Busca el sprite del color, tamaño y cuadro de este bus.
        // Ej: color 2, largo 3, cuadro 1 -> busgrande_1.png azul.
        ALLEGRO_BITMAP* sprite = sprites.getVehicle(bus.getColor(), length, busFrame);

        // Si no hay sprite (dato raro en el nivel), se salta este bus.
        // Ej: un color 8 no tiene imagen.
        if (sprite == nullptr) {
            continue;
        }

        // Escala para que una celda del sprite (64 px) mida lo mismo que una celda del tablero.
        // Ej: celda de 32 px -> escala 32 / 64 * 0.96 = 0.48.
        const float scale = cell / SPRITE_CELL * VEHICLE_FILL;

        // Dibuja el sprite centrado, escalado y girado según la dirección.
        // Ej: al_draw_scaled_rotated_bitmap(imagen, centroImgX, centroImgY, x, y, escalaX, escalaY, angulo, 0).
        al_draw_scaled_rotated_bitmap(sprite,
                                      al_get_bitmap_width(sprite) / 2.0f,
                                      al_get_bitmap_height(sprite) / 2.0f,
                                      centerX, centerY, scale, scale,
                                      directionAngle(direction), 0);
    }
}
