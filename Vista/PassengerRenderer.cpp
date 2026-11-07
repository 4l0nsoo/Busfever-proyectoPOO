#include "PassengerRenderer.h"
#include "SceneLayout.h"

// Trae std::sort y std::min.
// Ej: std::min(3.0f, 5.0f) da 3.
#include <algorithm>

// Trae cos(), sin() y sqrt() para el óvalo, la curva y las distancias.
// Ej: sqrt(3 * 3 + 4 * 4) = 5.
#include <cmath>

// Espacio de nombres anónimo: constantes que solo usa este archivo.
// Ej: otro .cpp no puede cambiar PERSON_SIZE.
namespace {

// Tamaño con el que se dibuja cada persona en pantalla.
// Ej: el sprite mide 32 px y se dibuja de 20 px.
const float PERSON_SIZE = 20.0f;

// Radio del "cuerpo" de cada persona para las colisiones.
// Ej: 6.5 -> dos personas no pueden tener sus centros a menos de 13 px.
const float BODY_RADIUS = 6.5f;

// Distancia mínima entre los centros de dos personas.
// Ej: 2 * 6.5 = 13 px.
const float MIN_DISTANCE = 2.0f * BODY_RADIUS;

// Velocidad máxima de una persona en píxeles por segundo.
// Ej: 85 -> en un segundo camina como máximo 85 px.
const float WALK_SPEED = 85.0f;

// Veces que se revisan los choques en cada cuadro (más veces = separación más precisa).
// Ej: 3 pasadas bastan para que nadie quede encimado.
const int COLLISION_PASSES = 3;

// Distancia a su puesto por debajo de la cual una persona se considera quieta.
// Ej: si le falta menos de 0.6 px, ya está esperando.
const float STOP_DISTANCE = 0.6f;

// Distancia entre una fila del camino y la siguiente.
// Ej: 13.5 px, un poco más que el cuerpo de una persona (13 px).
const float ROW_SPACING = 13.5f;

// Personas por fila en el camino y su separación a lo ancho.
// Ej: 3 personas a -16.5, 0 y +16.5 px del centro del camino.
const int PEOPLE_PER_ROW = 3;
const float ROW_OFFSETS[PEOPLE_PER_ROW] = {-16.5f, 0.0f, 16.5f};

// Puntos con los que se recorre la curva del camino.
// Ej: 30 tramos rectos pequeños parecen una curva.
const int CURVE_SAMPLES = 30;

// Altura hasta donde se calculan filas (un poco arriba de la ventana).
// Ej: y = -30 queda fuera de la pantalla; la fila sigue "más allá".
const float LANE_TOP_Y = -30.0f;

// Anillos de la glorieta: radio horizontal, radio vertical y cantidad de puestos de cada uno.
// Ej: el de afuera es más largo, por eso tiene más puestos (49) que el de adentro (31).
const int RING_ROWS = 3;
const float RING_RADII_X[RING_ROWS] = {98.0f, 122.0f, 146.0f};
const float RING_RADII_Y[RING_ROWS] = {47.0f, 65.0f, 83.0f};
const int RING_SLOT_COUNT[RING_ROWS] = {31, 40, 49};

// Óvalo prohibido para los que esperan: el anillo de afuera más el cuerpo de una persona.
// Ej: si un puesto del camino cae dentro de este óvalo, no se usa (ahí pasan los de la glorieta).
const float WAIT_LIMIT_X = RING_RADII_X[RING_ROWS - 1] + MIN_DISTANCE + 3.0f;
const float WAIT_LIMIT_Y = RING_RADII_Y[RING_ROWS - 1] + MIN_DISTANCE + 3.0f;

// Velocidad de giro de la glorieta en radianes por segundo.
// Ej: 0.35 -> da una vuelta completa en unos 18 segundos.
const float RING_SPEED = 0.35f;

// Duración de cada cuadro al caminar y al esperar.
// Ej: caminar es más rápido (0.14 s) que esperar (0.45 s).
const double WALK_SECONDS_PER_FRAME = 0.14;
const double IDLE_SECONDS_PER_FRAME = 0.45;

// Valor de pi como float.
// Ej: media vuelta = PI radianes.
const float PI = ALLEGRO_PI;

}

// Constructor: guarda las referencias, crea las animaciones y calcula los puestos.
// Ej: la glorieta empieza sin girar (ringAngle = 0) y sin caminantes.
PassengerRenderer::PassengerRenderer(const Queue& queue, const WaitingRoom& waitingRoom,
                                     const SpriteBank& sprites)
    : queue(queue), waitingRoom(waitingRoom), sprites(sprites),
      walkAnimation(SpriteBank::PERSON_FRAMES, WALK_SECONDS_PER_FRAME),
      idleAnimation(SpriteBank::PERSON_FRAMES, IDLE_SECONDS_PER_FRAME), ringAngle(0.0f) {
    // Calcula una sola vez los puestos de espera de los caminos.
    // Ej: no cambian entre niveles porque el fondo es el mismo.
    buildLaneRows();

    // Calcula una sola vez los puestos de la glorieta.
    // Ej: 120 puestos en 3 anillos.
    buildRingSlots();
}

void PassengerRenderer::buildLaneRows() {
    // Puntos que recorren el camino desde la glorieta hacia arriba.
    // Ej: (160, 216), (155, 215)... hasta (78, -60).
    std::vector<float> pathX;
    std::vector<float> pathY;

    // Recorre la curva de Bézier al revés (de t = 1 a t = 0) para empezar en la glorieta.
    // Ej: t = 1 es la entrada a la glorieta y t = 0 el final del tramo recto.
    for (int i = 0; i <= CURVE_SAMPLES; i++) {
        // Valor t de este punto de la curva.
        // Ej: i = 0 -> t = 1; i = 30 -> t = 0.
        const float t = 1.0f - static_cast<float>(i) / CURVE_SAMPLES;

        // Pesos de la fórmula de Bézier cúbica.
        // Ej: con t = 0 solo pesa el primer punto (el final del tramo recto).
        const float u = 1.0f - t;
        const float w0 = u * u * u;
        const float w1 = 3.0f * u * u * t;
        const float w2 = 3.0f * u * t * t;
        const float w3 = t * t * t;

        // Punto de la curva = suma de los 4 puntos de control multiplicados por sus pesos.
        // Ej: con t = 1 da exactamente la entrada (160, 216).
        pathX.push_back(w0 * SceneLayout::LANE_X + w1 * SceneLayout::LANE_CONTROL1_X +
                        w2 * SceneLayout::LANE_CONTROL2_X + w3 * SceneLayout::LANE_END_X);
        pathY.push_back(w0 * SceneLayout::LANE_BEND_Y + w1 * SceneLayout::LANE_CONTROL1_Y +
                        w2 * SceneLayout::LANE_CONTROL2_Y + w3 * SceneLayout::LANE_END_Y);
    }

    // Sigue por el tramo recto subiendo de 4 en 4 píxeles hasta salir de la pantalla.
    // Ej: (78, 116), (78, 112)... (78, -60).
    for (float y = SceneLayout::LANE_BEND_Y - 4.0f; y >= LANE_TOP_Y - 30.0f; y -= 4.0f) {
        pathX.push_back(static_cast<float>(SceneLayout::LANE_X));
        pathY.push_back(y);
    }

    // Distancia recorrida por el camino y distancia donde iría la siguiente fila.
    // Ej: se prueba una fila cada 13.5 px.
    float travelled = 0.0f;
    float nextRow = 0.0f;

    // Recorre cada tramo recto entre dos puntos seguidos del camino.
    // Ej: del punto 0 al 1, del 1 al 2, etc.
    for (std::size_t i = 1; i < pathX.size(); i++) {
        // Diferencia entre el punto anterior y el actual.
        // Ej: (78, 116) -> (78, 112) da dx = 0, dy = -4.
        const float dx = pathX[i] - pathX[i - 1];
        const float dy = pathY[i] - pathY[i - 1];

        // Largo del tramo (teorema de Pitágoras).
        // Ej: sqrt(0^2 + 4^2) = 4.
        const float length = std::sqrt(dx * dx + dy * dy);

        // Si el tramo no mide nada, se salta.
        // Ej: dos puntos iguales.
        if (length <= 0.0f) {
            continue;
        }

        // Prueba todas las filas que caen dentro de este tramo.
        // Ej: si el tramo va de 20 a 24 px y nextRow = 23, aquí se prueba una fila.
        while (nextRow <= travelled + length) {
            // Qué tan adentro del tramo está la fila (0 = inicio, 1 = final).
            // Ej: (23 - 20) / 4 = 0.75.
            const float t = (nextRow - travelled) / length;

            // Arma la fila: su centro y la dirección perpendicular al camino.
            // Ej: en el tramo recto la perpendicular es horizontal (1, 0).
            LaneRow row;
            row.x = pathX[i - 1] + dx * t;
            row.y = pathY[i - 1] + dy * t;
            row.normalX = -dy / length;
            row.normalY = dx / length;

            // La siguiente fila se prueba 13.5 px más arriba.
            // Ej: nextRow pasa de 23 a 36.5.
            nextRow += ROW_SPACING;

            // Si la fila ya quedó fuera de la pantalla por arriba, se terminó el camino.
            // Ej: no tiene sentido calcular filas en y = -200.
            if (row.y < LANE_TOP_Y) {
                return;
            }

            // Revisa si alguno de los 3 puestos de la fila cae dentro de la glorieta.
            // Ej: los puestos pegados a la entrada se descartan para no estorbar a los que giran.
            bool insideRing = false;
            for (float offset : ROW_OFFSETS) {
                // Posición del puesto a lo ancho de la fila.
                // Ej: centro (150, 214) + normal (0, 1) * 16.5 -> (150, 230.5).
                const float px = row.x + row.normalX * offset;
                const float py = row.y + row.normalY * offset;

                // Fórmula del óvalo: (dx / rx)^2 + (dy / ry)^2 < 1 significa "adentro".
                // Ej: el centro de la glorieta da 0 (adentro); un punto lejos da más de 1.
                const float ex = (px - SceneLayout::RING_CENTER_X) / WAIT_LIMIT_X;
                const float ey = (py - SceneLayout::RING_CENTER_Y) / WAIT_LIMIT_Y;
                if (ex * ex + ey * ey < 1.0f) {
                    insideRing = true;
                }
            }

            // Solo se guardan las filas que quedan completamente fuera de la glorieta.
            // Ej: la primera fila guardada es la que espera justo en el borde.
            if (!insideRing) {
                laneRows.push_back(row);
            }
        }

        // Suma el largo del tramo a la distancia recorrida.
        // Ej: 20 + 4 = 24 px recorridos.
        travelled += length;
    }
}

void PassengerRenderer::buildRingSlots() {
    // Recorre los 3 anillos de la glorieta.
    // Ej: ring = 0 (adentro), 1 (medio), 2 (afuera).
    for (int ring = 0; ring < RING_ROWS; ring++) {
        // Ángulo entre dos puestos seguidos de este anillo.
        // Ej: anillo de afuera: 2 * PI / 49 = 0.128 radianes.
        const float step = 2.0f * PI / RING_SLOT_COUNT[ring];

        // Crea cada puesto del anillo.
        // Ej: 49 puestos en el anillo de afuera.
        for (int k = 0; k < RING_SLOT_COUNT[ring]; k++) {
            // Ángulo de partida, corrido un tercio de paso por anillo para que no queden alineados.
            // Ej: el anillo del medio arranca 1/3 de paso después que el de adentro.
            RingSlot slot;
            slot.ring = ring;
            slot.baseAngle = (k + ring / 3.0f) * step;
            ringSlots.push_back(slot);
        }
    }

    // Ordena los puestos por ángulo: primero los que están frente al embarque.
    // Ej: así la persona 0 de la glorieta siempre queda junto al paso peatonal.
    std::sort(ringSlots.begin(), ringSlots.end(),
              [](const RingSlot& a, const RingSlot& b) { return a.baseAngle < b.baseAngle; });
}

void PassengerRenderer::targetOf(const Walker& walker, float& x, float& y) const {
    // Si el caminante está en la glorieta, su puesto gira con ella.
    // Ej: el puesto 0 con ringAngle = 0 está abajo, frente al embarque.
    if (walker.inRing) {
        // Puesto de la glorieta que le toca.
        // Ej: slot 10 -> anillo 1, ángulo de partida 0.47.
        const RingSlot& slot = ringSlots[walker.slot];

        // Ángulo actual: abajo (PI / 2) + su ángulo de partida + lo que ha girado la glorieta.
        // Ej: PI / 2 + 0.47 - 1.0.
        const float angle = PI / 2.0f + slot.baseAngle + ringAngle;

        // Punto del óvalo: centro + radio * coseno (x) y centro + radio * seno (y).
        // Ej: ángulo PI / 2 -> justo debajo del centro de la glorieta.
        x = SceneLayout::RING_CENTER_X + RING_RADII_X[slot.ring] * std::cos(angle);
        y = SceneLayout::RING_CENTER_Y + RING_RADII_Y[slot.ring] * std::sin(angle);
        return;
    }

    // Si espera en un camino: fila y puesto a lo ancho que le tocan.
    // Ej: slot 7 -> fila 2, puesto 1 (el del centro).
    const LaneRow& row = laneRows[walker.slot / PEOPLE_PER_ROW];
    const float offset = ROW_OFFSETS[walker.slot % PEOPLE_PER_ROW];

    // Posición en el camino izquierdo: centro de la fila + corrimiento a lo ancho.
    // Ej: centro (78, 100), normal (1, 0), corrimiento 16.5 -> (94.5, 100).
    x = row.x + row.normalX * offset;
    y = row.y + row.normalY * offset;

    // El camino derecho es el espejo del izquierdo.
    // Ej: x = 94.5 a la izquierda -> 576 - 94.5 = 481.5 a la derecha.
    if (walker.lane == 1) {
        x = SceneLayout::WIDTH - x;
    }
}

void PassengerRenderer::syncWithModel() {
    // Borra los caminantes del nivel anterior.
    // Ej: al pasar del nivel 1 al 3 se crean todos de nuevo.
    walkers.clear();

    // La glorieta vuelve a su posición inicial.
    // Ej: ringAngle = 0.
    ringAngle = 0.0f;

    // Número de la persona dentro de la glorieta (0 es la del embarque).
    // Ej: va de 0 a 119.
    int index = 0;

    // Crea un caminante por cada persona de la fila rotatoria.
    // Ej: [azul 8, rosado 16...] -> 8 caminantes azules, 16 rosados...
    for (const Passager& group : waitingRoom.getSeats()) {
        for (int p = 0; p < group.getGroupAmount(); p++) {
            // Si ya no hay puestos en la glorieta, los demás no se dibujan aquí.
            // Ej: no pasa con capacidad 120 y 120 puestos.
            if (index >= static_cast<int>(ringSlots.size())) {
                break;
            }

            // Arma el caminante de la glorieta con su puesto.
            // Ej: {color 2, puesto 5, en la glorieta}.
            Walker walker;
            walker.color = group.getColor();
            walker.frameOffset = index;
            walker.inRing = true;
            walker.slot = index;
            walker.lane = 0;
            walker.moving = true;

            // Aparece directamente en su puesto.
            // Ej: no empieza encimado con nadie.
            targetOf(walker, walker.x, walker.y);
            walkers.push_back(walker);
            index++;
        }
    }

    // Puestos de espera que caben en cada camino.
    // Ej: 20 filas x 3 = 60 puestos.
    const int capacity = static_cast<int>(laneRows.size()) * PEOPLE_PER_ROW;

    // Personas ya ubicadas en el camino izquierdo (0) y en el derecho (1).
    // Ej: {10, 8}.
    int filled[2] = {0, 0};

    // Número del grupo dentro de la fila estática.
    // Ej: el grupo 0 va al camino izquierdo y el 1 al derecho.
    int groupIndex = 0;

    // Crea caminantes para los grupos de la fila estática mientras quepan en pantalla.
    // Ej: [rojo 16, verde 8, azul 24...].
    for (const Passager& group : queue.getPassengers()) {
        // Si los dos caminos ya están llenos, no hace falta seguir.
        // Ej: el resto de la fila está "fuera de la pantalla".
        if (filled[0] >= capacity && filled[1] >= capacity) {
            break;
        }

        // Los grupos se alternan entre los dos caminos.
        // Ej: grupo 4 -> izquierda; grupo 5 -> derecha.
        const int lane = groupIndex % 2;

        // Crea un caminante por persona mientras quepa en su camino.
        // Ej: un grupo de 16 en un camino con 5 puestos libres: se crean 5.
        for (int p = 0; p < group.getGroupAmount() && filled[lane] < capacity; p++) {
            // Arma el caminante que espera en el camino.
            // Ej: {color 4, camino 1, puesto 7}.
            Walker walker;
            walker.color = group.getColor();
            walker.frameOffset = filled[lane] + lane;
            walker.inRing = false;
            walker.slot = filled[lane];
            walker.lane = lane;
            walker.moving = false;

            // Aparece directamente en su puesto de espera.
            // Ej: la fila de adelante queda pegada al borde de la glorieta.
            targetOf(walker, walker.x, walker.y);
            walkers.push_back(walker);
            filled[lane]++;
        }

        // Pasa al siguiente grupo.
        // Ej: groupIndex de 4 a 5.
        groupIndex++;
    }
}

void PassengerRenderer::update(double seconds) {
    // Avanza las dos animaciones.
    // Ej: caminar cambia de cuadro cada 0.14 s y esperar cada 0.45 s.
    walkAnimation.update(seconds);
    idleAnimation.update(seconds);

    // Gira la glorieta en contra del reloj (el ángulo baja).
    // Ej: en 1 segundo baja 0.35 radianes.
    ringAngle -= RING_SPEED * static_cast<float>(seconds);

    // Mantiene el ángulo entre -2*PI y 0.
    // Ej: -6.4 pasa a ser -0.12 (una vuelta después).
    if (ringAngle < -2.0f * PI) {
        ringAngle += 2.0f * PI;
    }

    // Lo máximo que puede caminar una persona en este cuadro.
    // Ej: 85 px/s * 0.016 s = 1.4 px.
    const float maxStep = WALK_SPEED * static_cast<float>(seconds);

    // Mueve a cada caminante hacia su puesto.
    // Ej: si el puesto se corrió 1 px (porque la glorieta giró), camina 1 px.
    for (Walker& walker : walkers) {
        // Puesto donde debería estar ahora.
        // Ej: (200, 260).
        float targetX, targetY;
        targetOf(walker, targetX, targetY);

        // Distancia que le falta para llegar.
        // Ej: de (198, 260) a (200, 260) faltan 2 px.
        const float dx = targetX - walker.x;
        const float dy = targetY - walker.y;
        const float distance = std::sqrt(dx * dx + dy * dy);

        // Si ya está prácticamente en su puesto, se queda quieto (esperando).
        // Ej: le faltan 0.3 px -> no se mueve.
        if (distance < STOP_DISTANCE) {
            walker.moving = false;
            continue;
        }

        // Avanza hacia el puesto sin pasarse de la velocidad máxima.
        // Ej: le faltan 2 px pero solo puede 1.4 -> avanza 1.4.
        const float step = std::min(distance, maxStep);
        walker.x += dx / distance * step;
        walker.y += dy / distance * step;
        walker.moving = true;
    }

    // Separa a los que quedaron chocando después de moverse.
    // Ej: nadie atraviesa a nadie, como si fueran cuerpos sólidos.
    resolveCollisions();
}

void PassengerRenderer::resolveCollisions() {
    // Repite la revisión varias veces: separar a dos puede acercar a otros.
    // Ej: 3 pasadas por cuadro.
    for (int pass = 0; pass < COLLISION_PASSES; pass++) {
        // Revisa cada pareja de caminantes una sola vez (i con j, j mayor que i).
        // Ej: con 4 caminantes se revisan (0,1) (0,2) (0,3) (1,2) (1,3) (2,3).
        for (std::size_t i = 0; i < walkers.size(); i++) {
            for (std::size_t j = i + 1; j < walkers.size(); j++) {
                // Diferencia de posición entre los dos.
                // Ej: A en (100, 100) y B en (108, 100) -> dx = 8, dy = 0.
                float dx = walkers[j].x - walkers[i].x;
                float dy = walkers[j].y - walkers[i].y;

                // Si están lejos (más de la distancia mínima), no hay choque.
                // Ej: se compara al cuadrado para no calcular la raíz si no hace falta.
                const float distanceSquared = dx * dx + dy * dy;
                if (distanceSquared >= MIN_DISTANCE * MIN_DISTANCE) {
                    continue;
                }

                // Distancia real entre los dos centros.
                // Ej: sqrt(64) = 8 px.
                float distance = std::sqrt(distanceSquared);

                // Si están exactamente en el mismo punto, se elige una dirección cualquiera.
                // Ej: se separan hacia los lados.
                if (distance < 0.001f) {
                    dx = 1.0f;
                    dy = 0.0f;
                    distance = 1.0f;
                }

                // Cuánto se están encimando, repartido entre los dos.
                // Ej: deben estar a 13 px y están a 8 -> se enciman 5 -> cada uno se corre 2.5.
                const float push = (MIN_DISTANCE - distance) / 2.0f;

                // Dirección que va del primero al segundo (vector unitario).
                // Ej: (8, 0) / 8 = (1, 0).
                const float nx = dx / distance;
                const float ny = dy / distance;

                // Corre al primero hacia atrás y al segundo hacia adelante.
                // Ej: A pasa de x = 100 a 97.5 y B de 108 a 110.5 -> quedan a 13 px.
                walkers[i].x -= nx * push;
                walkers[i].y -= ny * push;
                walkers[j].x += nx * push;
                walkers[j].y += ny * push;
            }
        }
    }
}

void PassengerRenderer::draw() const {
    // Lista de punteros a los caminantes para ordenarlos sin copiarlos.
    // Ej: 240 punteros.
    std::vector<const Walker*> ordered;
    for (const Walker& walker : walkers) {
        ordered.push_back(&walker);
    }

    // Ordena de arriba (y pequeño) a abajo (y grande).
    // Ej: la persona en y = 200 se dibuja antes que la de y = 250, que queda delante.
    std::sort(ordered.begin(), ordered.end(),
              [](const Walker* a, const Walker* b) { return a->y < b->y; });

    // Cuadro actual de cada animación.
    // Ej: caminar en el cuadro 2 y esperar en el cuadro 0.
    const int walkFrame = walkAnimation.getFrame();
    const int idleFrame = idleAnimation.getFrame();

    // Dibuja cada caminante.
    // Ej: los que se mueven usan la animación de caminar; los quietos, la de esperar.
    for (const Walker* walker : ordered) {
        // Cuadro que le toca, desfasado para que no se muevan todos igual.
        // Ej: (walkFrame + 3) % 4.
        const int baseFrame = walker->moving ? walkFrame : idleFrame;
        const int frame = (baseFrame + walker->frameOffset) % SpriteBank::PERSON_FRAMES;

        // Busca el sprite del color y cuadro de esta persona.
        // Ej: color 5, cuadro 2 -> personas/rosado/caminar_2.png.
        ALLEGRO_BITMAP* sprite = sprites.getPerson(walker->color, frame);

        // Si no hay sprite (color raro en el nivel), se salta.
        // Ej: color 9 no tiene imagen.
        if (sprite == nullptr) {
            continue;
        }

        // Dibuja el sprite escalado con los pies en (x, y).
        // Ej: al_draw_scaled_bitmap(imagen, origenX, origenY, anchoOrigen, altoOrigen, x, y, ancho, alto, 0).
        al_draw_scaled_bitmap(sprite, 0, 0,
                              al_get_bitmap_width(sprite), al_get_bitmap_height(sprite),
                              walker->x - PERSON_SIZE / 2.0f, walker->y - PERSON_SIZE * 0.9f,
                              PERSON_SIZE, PERSON_SIZE, 0);
    }
}
