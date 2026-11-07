/**
 * @file PassengerRenderer.h
 * @brief Definición de la clase PassengerRenderer
 *
 * @details
 * Dibuja y anima a las personas del nivel como caminantes con física sencilla:
 * cada persona es un círculo que no puede atravesar a otra (detección y
 * resolución de colisiones). Las de la fila rotatoria (WaitingRoom) caminan
 * alrededor de la glorieta en sentido contrario al reloj, cada una hacia su
 * puesto; las de la fila estática (Queue) esperan en los dos caminos, detenidas
 * en el borde de la glorieta, sin meterse mientras la glorieta esté llena.
 * Solo lee el modelo: no decide quién sube a un bus.
 *
 * -----------------------------------------------------------------------------
 * Estándar CRC (Clase - Responsabilidad - Colaboración)
 * -----------------------------------------------------------------------------
 *
 * Clase:
 *   PassengerRenderer (implementa IRenderable)
 *
 * Responsabilidades:
 *  - Crear un caminante por cada persona visible del modelo (syncWithModel).
 *  - Calcular los puestos de espera en los caminos, fuera de la glorieta.
 *  - Calcular los puestos de la glorieta, separados para que nadie se encime.
 *  - Mover a cada caminante hacia su puesto con una velocidad máxima.
 *  - Detectar y separar a los caminantes que se chocan (colisiones).
 *  - Dibujar a todos ordenados de arriba a abajo, caminando o esperando.
 *
 * Colaboradores:
 *  - Queue, WaitingRoom, Passager:
 *      * De ahí lee los grupos de personas, su color y su cantidad.
 *  - SpriteBank:
 *      * Le entrega los sprites de las personas.
 *  - Animation:
 *      * Le dice qué cuadro de caminar (o de esperar) toca.
 *  - SceneLayout:
 *      * Le da la forma de los caminos y de la glorieta.
 *
 * -----------------------------------------------------------------------------
 *
 * @author Santiago Caicedo Cabrera, Gabriela Castaño Castillo, Alonso Javier Diaz Veliz, Jesus Rodrigo Arango Arias, Luis Felipe Mosquera Machado
 * @email santiago.caicedo.cabrera@correounivalle.edu.co, gabriela.castano@correounivalle.edu.co, alonso.diaz@correounivalle.edu.co, arango.jesus@correounivalle.edu.co
 */

#pragma once
#include "IRenderable.h"
#include "Animation.h"
#include "SpriteBank.h"
#include "../Modelo/Queue.h"
#include "../Modelo/WaitingRoom.h"
#include <vector>

class PassengerRenderer : public IRenderable {
private:
    // Una fila de puestos en el camino: su centro y hacia dónde se acomodan a lo ancho.
    // Ej: centro (78, 100) y normal (1, 0) -> las 3 personas quedan en x = 61.5, 78 y 94.5.
    struct LaneRow {
        float x;
        float y;
        float normalX;
        float normalY;
    };

    // Un puesto de la glorieta: en qué anillo está y su ángulo de partida.
    // Ej: {2, 0.13} -> anillo de afuera, un poco detrás del embarque.
    struct RingSlot {
        int ring;
        float baseAngle;
    };

    // Un caminante: una persona con posición propia que se mueve y choca con las demás.
    // Ej: persona roja en (150, 230) que va hacia el puesto 5 de la glorieta.
    struct Walker {
        float x;
        float y;
        int color;
        int frameOffset;
        bool inRing;
        int slot;
        int lane;
        bool moving;
    };

    // Fila estática del modelo (personas esperando en los caminos).
    // Ej: en el nivel fácil quedan 736 personas fuera de la glorieta.
    const Queue& queue;

    // Fila rotatoria del modelo (personas en la glorieta).
    // Ej: hasta 120 personas.
    const WaitingRoom& waitingRoom;

    // Banco de sprites de donde salen las imágenes de las personas.
    // Ej: sprites.getPerson(4, 1) -> persona verde en el cuadro 1.
    const SpriteBank& sprites;

    // Animación de caminar (rápida) para quien se está moviendo.
    // Ej: cambia de cuadro cada 0.14 segundos.
    Animation walkAnimation;

    // Animación de esperar (lenta) para quien está quieto en la fila.
    // Ej: cambia de cuadro cada 0.45 segundos, como alguien impaciente.
    Animation idleAnimation;

    // Ángulo que ha girado la glorieta (en radianes); baja para girar en contra del reloj.
    // Ej: después de un rato vale -3.14 (media vuelta).
    float ringAngle;

    // Puestos de espera del camino izquierdo (el derecho es su espejo), todos fuera de la glorieta.
    // Ej: la fila 0 es la que queda pegada al borde de la glorieta.
    std::vector<LaneRow> laneRows;

    // Puestos de la glorieta ordenados desde el embarque hacia atrás.
    // Ej: el puesto 0 está frente al paso peatonal.
    std::vector<RingSlot> ringSlots;

    // Todos los caminantes del nivel actual.
    // Ej: 120 en la glorieta + los que quepan en los caminos.
    std::vector<Walker> walkers;

    // Calcula los puestos de espera siguiendo la forma del camino, sin entrar a la glorieta.
    // Ej: se llama una sola vez en el constructor.
    void buildLaneRows();

    // Calcula los puestos de la glorieta, más puestos en los anillos más grandes.
    // Ej: 31 adentro, 40 en el medio y 49 afuera = 120.
    void buildRingSlots();

    // Retorna dónde debe estar ahora un caminante (su puesto, que en la glorieta gira).
    // Ej: un caminante del camino derecho, puesto 4 -> (481.5, 150).
    void targetOf(const Walker& walker, float& x, float& y) const;

    // Separa a los caminantes que se están chocando.
    // Ej: si dos están a 8 px y deben estar a 13 px, cada uno se corre 2.5 px.
    void resolveCollisions();

public:
    // Recibe la fila estática, la fila rotatoria y el banco de sprites.
    // Ej: PassengerRenderer passengers(queue, waitingRoom, sprites);
    PassengerRenderer(const Queue& queue, const WaitingRoom& waitingRoom, const SpriteBank& sprites);

    // Vuelve a crear los caminantes leyendo el modelo (se llama al cargar un nivel).
    // Ej: después de controller.loadLevel(...), passengers.syncWithModel();
    void syncWithModel();

    // Mueve a los caminantes, resuelve choques y avanza las animaciones.
    // Ej: passengers.update(1.0 / 60.0);
    void update(double seconds) override;

    // Dibuja a todos los caminantes ordenados de arriba a abajo.
    // Ej: passengers.draw(); después del fondo y antes de los buses.
    void draw() const override;
};
