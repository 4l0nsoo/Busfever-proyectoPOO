/**
 * @file Animation.h
 * @brief Definición de la clase Animation
 *
 * @details
 * Lleva la cuenta de qué cuadro (frame) de una animación toca mostrar según el
 * tiempo que ha pasado. No dibuja nada: solo dice el número del cuadro. Así la
 * misma clase sirve para las personas caminando y para los buses.
 *
 * -----------------------------------------------------------------------------
 * Estándar CRC (Clase - Responsabilidad - Colaboración)
 * -----------------------------------------------------------------------------
 *
 * Clase:
 *   Animation
 *
 * Responsabilidades:
 *  - Guardar cuántos cuadros tiene la animación y cuánto dura cada uno.
 *  - Acumular el tiempo y avanzar al siguiente cuadro (volviendo al primero al final).
 *  - Decir cuál es el cuadro actual.
 *
 * Colaboradores:
 *  - PassengerRenderer:
 *      * Usa una Animation de 4 cuadros para las personas caminando.
 *  - BoardRenderer:
 *      * Usa una Animation de 2 cuadros para los buses.
 *
 * -----------------------------------------------------------------------------
 *
 * @author Santiago Caicedo Cabrera, Gabriela Castaño Castillo, Alonso Javier Diaz Veliz, Jesus Rodrigo Arango Arias, Luis Felipe Mosquera Machado
 * @email santiago.caicedo.cabrera@correounivalle.edu.co, gabriela.castano@correounivalle.edu.co, alonso.diaz@correounivalle.edu.co, arango.jesus@correounivalle.edu.co
 */

#pragma once

class Animation {
private:
    // Cantidad de cuadros de la animación.
    // Ej: 4 para caminar (pie izquierdo, quieto, pie derecho, quieto).
    int frameCount;

    // Segundos que dura cada cuadro en pantalla.
    // Ej: 0.15 -> se cambia de cuadro unas 6 o 7 veces por segundo.
    double secondsPerFrame;

    // Tiempo acumulado desde que empezó el cuadro actual.
    // Ej: si llega a 0.15, se pasa al siguiente cuadro.
    double elapsed;

    // Número del cuadro actual (empieza en 0).
    // Ej: va 0, 1, 2, 3, 0, 1, 2, 3...
    int currentFrame;

public:
    // Crea la animación con su cantidad de cuadros y la duración de cada uno.
    // Ej: Animation walk(4, 0.15);
    Animation(int frameCount, double secondsPerFrame);

    // Suma el tiempo que pasó y avanza de cuadro cuando toca.
    // Ej: walk.update(1.0 / 60.0); en cada tic del temporizador.
    void update(double seconds);

    // Retorna el número del cuadro que toca mostrar.
    // Ej: 2 -> se dibuja caminar_2.png.
    int getFrame() const;

    // Retorna cuántos cuadros tiene la animación.
    // Ej: 4 para caminar, 2 para los buses.
    int getFrameCount() const;
};
