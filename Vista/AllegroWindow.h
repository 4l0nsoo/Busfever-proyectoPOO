/**
 * @file AllegroWindow.h
 * @brief Definición de la clase AllegroWindow
 *
 * @details
 * Encapsula todo lo que Allegro necesita para abrir la ventana del juego:
 * encender la librería y sus módulos, crear la ventana con un tamaño que quepa
 * en la pantalla, crear la cola de eventos y el temporizador, y liberar todo al
 * final. Así el resto del programa no tiene que repetir estos pasos.
 *
 * -----------------------------------------------------------------------------
 * Estándar CRC (Clase - Responsabilidad - Colaboración)
 * -----------------------------------------------------------------------------
 *
 * Clase:
 *   AllegroWindow
 *
 * Responsabilidades:
 *  - Iniciar Allegro y sus módulos (teclado e imágenes).
 *  - Crear la ventana escalada al tamaño de la pantalla del usuario.
 *  - Entregar los eventos (teclas, cerrar ventana, temporizador) al programa.
 *  - Mostrar en pantalla lo que se dibujó.
 *  - Liberar la memoria de la ventana, la cola y el temporizador al terminar.
 *
 * Colaboradores:
 *  - SceneLayout:
 *      * Le da el tamaño lógico (576 x 1024) con el que se diseñó el escenario.
 *  - Scenery:
 *      * Dibuja el fondo sobre la ventana que crea AllegroWindow.
 *
 * -----------------------------------------------------------------------------
 *
 * @author Santiago Caicedo Cabrera, Gabriela Castaño Castillo, Alonso Javier Diaz Veliz, Jesus Rodrigo Arango Arias, Luis Felipe Mosquera Machado
 * @email santiago.caicedo.cabrera@correounivalle.edu.co, gabriela.castano@correounivalle.edu.co, alonso.diaz@correounivalle.edu.co, arango.jesus@correounivalle.edu.co
 */

#pragma once

// Trae lo básico de Allegro: ventana, eventos, teclado y temporizador.
// Ej: sin este include, el compilador no sabe qué es ALLEGRO_DISPLAY.
#include <allegro5/allegro.h>

// Trae std::string para recibir el título de la ventana.
// Ej: "Bus Fever Party! FPOO-C++ Edition".
#include <string>

class AllegroWindow {
private:
    // Puntero a la ventana del juego (nullptr mientras no se haya creado).
    // Ej: al_create_display(576, 1024) devuelve este puntero.
    ALLEGRO_DISPLAY* display;

    // Fila donde Allegro anota lo que pasa: teclas, cerrar ventana, temporizador...
    // Ej: si el usuario presiona ESC, llega un evento ALLEGRO_EVENT_KEY_DOWN.
    ALLEGRO_EVENT_QUEUE* eventQueue;

    // Reloj que avisa cada cierto tiempo que hay que volver a dibujar.
    // Ej: a 60 cuadros por segundo avisa cada 1/60 = 0.016 segundos.
    ALLEGRO_TIMER* timer;

    // Cuánto se agranda o reduce el dibujo para que quepa en la pantalla.
    // Ej: 1.0 = tamaño real; 0.7 = la ventana se ve al 70 %.
    float scale;

    // Cambia la carpeta de trabajo a la carpeta del proyecto (donde está assets).
    // Ej: así "assets/escenario.png" se encuentra aunque el .exe esté dentro de Vista/.
    void useProjectFolder();

    // Calcula la escala para que la ventana quepa a lo alto de la pantalla.
    // Ej: en una pantalla de 768 px de alto la ventana se reduce a ~0.65.
    float computeScale() const;

public:
    // Constructor: deja todo en nullptr, todavía no abre nada.
    // Ej: AllegroWindow window; crea el objeto sin mostrar la ventana.
    AllegroWindow();

    // Destructor: destruye el temporizador, la cola y la ventana.
    // Ej: se llama solo cuando window sale del main.
    ~AllegroWindow();

    // Se prohíbe copiar la ventana: dos objetos destruirían la misma ventana.
    // Ej: AllegroWindow otra = window; da error de compilación (y está bien que lo dé).
    AllegroWindow(const AllegroWindow&) = delete;

    // Se prohíbe asignar una ventana a otra por la misma razón.
    // Ej: otra = window; también da error de compilación.
    AllegroWindow& operator=(const AllegroWindow&) = delete;

    // Enciende Allegro y abre la ventana; retorna false si algo falla.
    // Ej: if (!window.initialize("Mi juego")) return 1;
    bool initialize(const std::string& title);

    // Pone a andar el temporizador de 60 cuadros por segundo.
    // Ej: se llama justo antes de entrar al ciclo principal del juego.
    void startTimer();

    // Espera (sin gastar procesador) a que llegue un evento y lo guarda.
    // Ej: window.waitForEvent(event); luego se revisa event.type.
    void waitForEvent(ALLEGRO_EVENT& event);

    // Dice si ya no quedan eventos pendientes en la cola.
    // Ej: se dibuja solo cuando la cola está vacía, para no atrasarse.
    bool isEventQueueEmpty() const;

    // Muestra en pantalla todo lo que se dibujó "a escondidas".
    // Ej: sin esto la ventana se queda en negro.
    void present();

    // Retorna la escala con la que se está mostrando la ventana.
    // Ej: 0.77 si la ventana se ve al 77 % del tamaño real.
    float getScale() const;
};
