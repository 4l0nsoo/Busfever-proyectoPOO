/**
 * @file BoardRenderer.h
 * @brief Definición de la clase BoardRenderer
 *
 * @details
 * Dibuja el tablero de tráfico dentro del panel de abajo: las celdas de la
 * cuadrícula y cada vehículo del nivel con su sprite, su color, su tamaño y
 * girado según la dirección hacia la que sale. Solo lee el modelo (Traffic):
 * no mueve buses ni decide reglas.
 *
 * -----------------------------------------------------------------------------
 * Estándar CRC (Clase - Responsabilidad - Colaboración)
 * -----------------------------------------------------------------------------
 *
 * Clase:
 *   BoardRenderer (implementa IRenderable)
 *
 * Responsabilidades:
 *  - Calcular el tamaño de cada celda para que el tablero quepa en el panel.
 *  - Dibujar las celdas de la cuadrícula.
 *  - Dibujar cada bus en sus celdas, girado según su dirección.
 *  - Animar los buses (2 cuadros que se alternan).
 *
 * Colaboradores:
 *  - Traffic, Bus:
 *      * De ahí lee el tamaño del tablero y los datos de cada bus.
 *  - SpriteBank:
 *      * Le entrega el sprite de cada vehículo.
 *  - Animation:
 *      * Le dice qué cuadro de la animación toca.
 *  - SceneLayout:
 *      * Le da el rectángulo del panel del tablero.
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
#include "../Modelo/Traffic.h"

class BoardRenderer : public IRenderable {
private:
    // Tablero del modelo que se va a dibujar (solo lectura).
    // Ej: traffic.getBuses() trae los 34 buses del nivel fácil.
    const Traffic& traffic;

    // Banco de sprites de donde salen las imágenes de los vehículos.
    // Ej: sprites.getVehicle(1, 2, 0) -> bus mediano rojo.
    const SpriteBank& sprites;

    // Animación de los buses: 2 cuadros que se alternan.
    // Ej: el motor "vibra" y sale humo cada medio segundo.
    Animation vehicleAnimation;

    // Calcula el tamaño de una celda en píxeles para que el tablero quepa en el panel.
    // Ej: tablero de 8 x 8 -> celdas de 64 px; de 16 x 16 -> celdas de 30 px.
    float computeCellSize() const;

    // Retorna el ángulo de giro del sprite según la dirección del bus.
    // Ej: "derecha" -> 0; "abajo" -> 90 grados; "arriba" -> -90 grados (en radianes).
    static float directionAngle(const std::string& direction);

public:
    // Recibe el tablero y el banco de sprites.
    // Ej: BoardRenderer board(traffic, sprites);
    BoardRenderer(const Traffic& traffic, const SpriteBank& sprites);

    // Avanza la animación de los buses.
    // Ej: board.update(1.0 / 60.0);
    void update(double seconds) override;

    // Dibuja las celdas y todos los buses del tablero.
    // Ej: board.draw(); después de dibujar el fondo.
    void draw() const override;
};
