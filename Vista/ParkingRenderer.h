/**
 * @file ParkingRenderer.h
 * @brief Definición de la clase ParkingRenderer
 *
 * @details
 * Dibuja en la ventana los buses que están en el parqueadero del modelo. Así,
 * cuando en la consola se escribe el ID de un bus y el Controller lo manda al
 * estacionamiento, la ventana de Allegro lo muestra en su puesto: la ventana
 * refleja exactamente lo que pasó en la consola. Solo lee el modelo (Parking).
 *
 * -----------------------------------------------------------------------------
 * Estándar CRC (Clase - Responsabilidad - Colaboración)
 * -----------------------------------------------------------------------------
 *
 * Clase:
 *   ParkingRenderer (implementa IRenderable)
 *
 * Responsabilidades:
 *  - Dibujar cada bus del parqueadero en su puesto (el primero en el puesto 1, etc.).
 *  - Escalar el bus para que quepa en el puesto y dibujarlo mirando hacia arriba.
 *  - Animar los buses estacionados (2 cuadros).
 *
 * Colaboradores:
 *  - Parking, Bus:
 *      * De ahí lee los buses estacionados.
 *  - SpriteBank:
 *      * Le entrega los sprites de los vehículos.
 *  - Animation:
 *      * Le dice qué cuadro de la animación toca.
 *  - SceneLayout:
 *      * Le da la posición de los 4 puestos.
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
#include "../Modelo/Parking.h"

class ParkingRenderer : public IRenderable {
private:
    // Parqueadero del modelo (solo lectura).
    // Ej: parking.getBuses() trae los buses que la consola mandó al estacionamiento.
    const Parking& parking;

    // Banco de sprites de los vehículos.
    // Ej: sprites.getVehicle(2, 1, 0) -> carro azul.
    const SpriteBank& sprites;

    // Animación de los buses estacionados (2 cuadros).
    // Ej: el motor vibra mientras esperan.
    Animation vehicleAnimation;

public:
    // Recibe el parqueadero y el banco de sprites.
    // Ej: ParkingRenderer parkingView(parking, sprites);
    ParkingRenderer(const Parking& parking, const SpriteBank& sprites);

    // Avanza la animación de los buses estacionados.
    // Ej: parkingView.update(1.0 / 60.0);
    void update(double seconds) override;

    // Dibuja los buses estacionados en sus puestos.
    // Ej: parkingView.draw(); después del tablero.
    void draw() const override;
};
