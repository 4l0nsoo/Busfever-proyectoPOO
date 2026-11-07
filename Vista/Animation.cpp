#include "Animation.h"

// Constructor: guarda los datos y empieza en el primer cuadro.
// Ej: Animation(2, 0.4) -> 2 cuadros de 0.4 segundos cada uno.
Animation::Animation(int frameCount, double secondsPerFrame)
    : frameCount(frameCount), secondsPerFrame(secondsPerFrame), elapsed(0.0), currentFrame(0) {}

void Animation::update(double seconds) {
    // Si la animación no tiene cuadros o no tiene duración, no hay nada que avanzar.
    // Ej: protege de dividir el tiempo entre 0.
    if (frameCount <= 0 || secondsPerFrame <= 0.0) {
        return;
    }

    // Suma el tiempo que pasó desde el cuadro anterior.
    // Ej: 0.10 + 0.016 = 0.116 segundos.
    elapsed += seconds;

    // Mientras se haya pasado del tiempo de un cuadro, avanza al siguiente.
    // Ej: con 0.15 por cuadro y 0.16 acumulado, avanza un cuadro y quedan 0.01.
    while (elapsed >= secondsPerFrame) {
        // Descuenta el tiempo del cuadro que terminó.
        // Ej: 0.16 - 0.15 = 0.01.
        elapsed -= secondsPerFrame;

        // Pasa al siguiente cuadro y vuelve a 0 después del último (operador %).
        // Ej: con 4 cuadros: 3 + 1 = 4 -> 4 % 4 = 0.
        currentFrame = (currentFrame + 1) % frameCount;
    }
}

int Animation::getFrame() const {
    // Retorna el cuadro actual.
    // Ej: 1.
    return currentFrame;
}

int Animation::getFrameCount() const {
    // Retorna la cantidad de cuadros.
    // Ej: 4.
    return frameCount;
}
