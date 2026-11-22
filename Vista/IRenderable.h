/**
 * @file IRenderable.h
 * @brief Definición de la interfaz IRenderable
 *
 * @details
 * Contrato para todo lo que se dibuja en la ventana: el fondo, las personas y
 * los buses. Cada elemento sabe actualizar su animación y dibujarse. La ventana
 * guarda una lista de IRenderable y los recorre sin saber de qué tipo es cada uno
 * (polimorfismo).
 *
 * -----------------------------------------------------------------------------
 * Estándar CRC (Clase - Responsabilidad - Colaboración)
 * -----------------------------------------------------------------------------
 *
 * Clase:
 *   IRenderable (interfaz, solo métodos virtuales puros)
 *
 * Responsabilidades:
 *  - Definir update(): avanzar las animaciones según el tiempo que pasó.
 *  - Definir draw(): dibujar el elemento en la ventana.
 *
 * Colaboradores:
 *  - Scenery, PassengerRenderer, BoardRenderer:
 *      * Implementan la interfaz, cada uno a su manera.
 *  - SceneryWindow:
 *      * Recorre una lista de IRenderable para actualizarlos y dibujarlos.
 *
 * Principios: abierto/cerrado (para dibujar algo nuevo se crea otra clase hija
 * sin cambiar SceneryWindow) y sustitución de Liskov (cualquier hija sirve donde
 * se espera un IRenderable).
 *
 * -----------------------------------------------------------------------------
 *
 * @author Santiago Caicedo Cabrera, Gabriela Castaño Castillo, Alonso Javier Diaz Veliz, Jesus Rodrigo Arango Arias, Luis Felipe Mosquera Machado
 * @email santiago.caicedo.cabrera@correounivalle.edu.co, gabriela.castano@correounivalle.edu.co, alonso.diaz@correounivalle.edu.co, arango.jesus@correounivalle.edu.co
 */

#pragma once

class IRenderable {
public:
    // Destructor virtual: destruye bien una clase hija desde un puntero a la interfaz.
    // Ej: si se borra un IRenderable* que es un BoardRenderer, se llama al destructor correcto.
    virtual ~IRenderable() = default;

    // Avanza las animaciones según los segundos que pasaron desde el cuadro anterior.
    // Ej: update(1.0 / 60.0) avanza 0.016 segundos.
    virtual void update(double seconds) = 0;

    // Dibuja el elemento en la ventana.
    // Ej: el fondo se dibuja primero y los buses al final, encima de todo.
    virtual void draw() const = 0;
};
