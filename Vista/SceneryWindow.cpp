#include "SceneryWindow.h"
#include "AllegroWindow.h"
#include "Scenery.h"

// Constructor: guarda el título y la ruta en los atributos.
// Ej: SceneryWindow("Nivel 1", "assets/escenario.png") guarda esos dos textos.
SceneryWindow::SceneryWindow(const std::string& title, const std::string& backgroundPath)
    : title(title), backgroundPath(backgroundPath) {}

bool SceneryWindow::run() {
    // Crea el objeto ventana (todavía no se ve nada).
    // Ej: es como sacar el televisor de la caja sin encenderlo.
    // Se crea dentro de run() para que la ventana se cierre sola cuando run() termine.
    AllegroWindow window;

    // Enciende Allegro y abre la ventana; si falla, se avisa con false.
    // Ej: el main puede mostrar un mensaje de error en la consola.
    if (!window.initialize(title)) {
        return false;
    }

    // Crea el objeto que sabe dibujar el fondo.
    // Ej: guarda la imagen escenario.png en memoria.
    Scenery scenery;

    // Carga la imagen de fondo; si no la encuentra, se avisa con false.
    // Ej: falla si no existe la carpeta assets junto al .exe.
    if (!scenery.loadBackground(backgroundPath)) {
        return false;
    }

    // Indica si la ventana sigue abierta.
    // Ej: se pone en false al cerrar la ventana o presionar ESC.
    bool running = true;

    // Indica si toca volver a dibujar la pantalla.
    // Ej: empieza en true para dibujar apenas se abre la ventana.
    bool redraw = true;

    // Arranca el reloj de 60 cuadros por segundo.
    // Ej: desde aquí llegan eventos ALLEGRO_EVENT_TIMER.
    window.startTimer();

    // Ciclo principal: se repite mientras la ventana esté abierta.
    // Ej: cada vuelta atiende un evento y, si toca, redibuja.
    while (running) {
        // Variable donde se guarda el evento que llegó.
        // Ej: puede ser una tecla, cerrar la ventana o un tic del reloj.
        ALLEGRO_EVENT event;

        // Espera (sin gastar procesador) a que llegue un evento.
        // Ej: si no pasa nada, llega el siguiente tic en 1/60 de segundo.
        window.waitForEvent(event);

        // Si el usuario presionó la X de la ventana, se sale del ciclo.
        // Ej: ALLEGRO_EVENT_DISPLAY_CLOSE llega al hacer clic en la X.
        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            running = false;
        }
        // Si presionó la tecla ESC, también se sale.
        // Ej: event.keyboard.keycode dice cuál tecla fue.
        else if (event.type == ALLEGRO_EVENT_KEY_DOWN &&
                 event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
            running = false;
        }
        // Si fue un tic del reloj, toca redibujar.
        // Ej: 60 veces por segundo.
        else if (event.type == ALLEGRO_EVENT_TIMER) {
            redraw = true;
        }

        // Dibuja solo si toca y si no quedan eventos pendientes.
        // Ej: si llegaron 3 tics juntos, se dibuja una sola vez, no tres.
        if (redraw && window.isEventQueueEmpty()) {
            // Ya se va a dibujar, así que se baja la bandera.
            // Ej: volverá a true con el siguiente tic.
            redraw = false;

            // Dibuja la imagen de fondo.
            // Ej: ocupa toda la ventana.
            scenery.draw();

            // Muestra en pantalla lo que se dibujó.
            // Ej: sin esta línea la ventana se queda en negro.
            window.present();
        }
    }

    // La ventana se cerró bien.
    // Ej: al salir de run(), los destructores de scenery y window liberan la memoria.
    return true;
}
