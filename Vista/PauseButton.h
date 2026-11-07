/**
 * @file PauseButton.h
 * @brief Definición de la clase PauseButton
 *
 * @details
 * Botón redondo azul con el símbolo de pausa (dos barras) que se ve en la
 * esquina de arriba a la derecha mientras se juega. Al hacerle clic se abre el
 * menú de pausa. Solo dibuja el botón y dice si un punto está encima de él.
 *
 * -----------------------------------------------------------------------------
 * Estándar CRC (Clase - Responsabilidad - Colaboración)
 * -----------------------------------------------------------------------------
 *
 * Clase:
 *   PauseButton (implementa IRenderable)
 *
 * Responsabilidades:
 *  - Dibujar el botón de pausa con relieve.
 *  - Iluminarse cuando el mouse está encima.
 *  - Decir si un punto (un clic) cae sobre el botón.
 *
 * Colaboradores:
 *  - IRenderable:
 *      * Es la interfaz que implementa para dibujarse como una capa más.
 *  - SceneryWindow:
 *      * Le pasa la posición del mouse y le pregunta si se hizo clic encima.
 *
 * -----------------------------------------------------------------------------
 *
 * @author Santiago Caicedo Cabrera, Gabriela Castaño Castillo, Alonso Javier Diaz Veliz, Jesus Rodrigo Arango Arias, Luis Felipe Mosquera Machado
 * @email santiago.caicedo.cabrera@correounivalle.edu.co, gabriela.castano@correounivalle.edu.co, alonso.diaz@correounivalle.edu.co, arango.jesus@correounivalle.edu.co
 */

#pragma once
#include "IRenderable.h"

class PauseButton : public IRenderable {
private:
    // Centro del botón en x y en y.
    // Ej: (530, 46) -> esquina de arriba a la derecha.
    float centerX;
    float centerY;

    // Indica si el mouse está encima del botón.
    // Ej: true -> el botón se ve más claro.
    bool hovered;

public:
    // Crea el botón en la esquina de arriba a la derecha.
    // Ej: PauseButton pauseButton;
    PauseButton();

    // Recibe la posición del mouse (en medidas del diseño) para iluminarse.
    // Ej: pauseButton.setMousePosition(530, 46); -> se ilumina.
    void setMousePosition(float x, float y);

    // Retorna true si el punto (x, y) está sobre el botón.
    // Ej: contains(530, 46) -> true; contains(100, 500) -> false.
    bool contains(float x, float y) const;

    // El botón no tiene animación, así que no hace nada.
    // Ej: pauseButton.update(0.016); no cambia nada.
    void update(double seconds) override;

    // Dibuja el botón azul con las dos barras blancas.
    // Ej: pauseButton.draw(); después de los buses.
    void draw() const override;
};
