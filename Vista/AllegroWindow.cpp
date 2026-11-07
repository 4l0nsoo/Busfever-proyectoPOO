#include "AllegroWindow.h"
#include "SceneLayout.h"

// Módulo para cargar imágenes .png y .jpg.
// Ej: sin él, al_load_bitmap("assets/escenario.png") devuelve nullptr.
#include <allegro5/allegro_image.h>

// Trae std::min para escoger el menor de dos números.
// Ej: std::min(1.0f, 0.7f) da 0.7.
#include <algorithm>

// Cuántas veces por segundo se redibuja la ventana.
// Ej: 60 es lo normal en juegos; 30 se vería un poco "a saltos".
const double FRAMES_PER_SECOND = 60.0;

// Píxeles que se dejan libres para la barra de tareas y el borde de la ventana.
// Ej: en una pantalla de 1080 px de alto, la ventana medirá como máximo 1080 - 110 = 970.
const int SCREEN_MARGIN = 110;

// Constructor: aún no hay ventana, cola ni temporizador.
// Ej: los punteros en nullptr indican "todavía no existe".
AllegroWindow::AllegroWindow()
    : display(nullptr), eventQueue(nullptr), timer(nullptr), scale(1.0f) {}

AllegroWindow::~AllegroWindow() {
    // Destruye el temporizador si se llegó a crear.
    // Ej: si initialize falló antes de crearlo, timer sigue en nullptr y no se toca.
    if (timer != nullptr) {
        al_destroy_timer(timer);
    }

    // Destruye la cola de eventos si existe.
    // Ej: todo lo que se crea con al_create_... se destruye con al_destroy_...
    if (eventQueue != nullptr) {
        al_destroy_event_queue(eventQueue);
    }

    // Cierra y destruye la ventana de último, porque lo demás depende de ella.
    // Ej: es como apagar primero los aparatos y luego la luz de la casa.
    if (display != nullptr) {
        al_destroy_display(display);
    }
}

void AllegroWindow::useProjectFolder() {
    // Pide a Allegro la ruta de la carpeta donde está el .exe.
    // Ej: C:/Users/.../PROYECTO 1 POO/Vista/
    ALLEGRO_PATH* folder = al_get_standard_path(ALLEGRO_RESOURCES_PATH);

    // Cambia la carpeta de trabajo a esa ruta.
    // Ej: ahora las rutas relativas se buscan dentro de Vista/.
    al_change_directory(al_path_cstr(folder, ALLEGRO_NATIVE_PATH_SEP));

    // Libera la memoria de la ruta, ya no se necesita.
    // Ej: igual que destruir la ventana, pero con un objeto más pequeño.
    al_destroy_path(folder);

    // Si aquí no está la carpeta assets, sube una carpeta (a la del proyecto).
    // Ej: de PROYECTO 1 POO/Vista/ pasa a PROYECTO 1 POO/, donde están assets y los niveles .txt.
    if (!al_filename_exists("assets")) {
        al_change_directory("..");
    }
}

float AllegroWindow::computeScale() const {
    // Variable donde Allegro guarda las medidas del monitor principal.
    // Ej: info.x1 = 0, info.y1 = 0, info.x2 = 1920, info.y2 = 1080.
    ALLEGRO_MONITOR_INFO info;

    // Si no se pueden leer las medidas, se usa el tamaño real (escala 1).
    // Ej: pasa muy pocas veces, pero así el programa no se rompe.
    if (!al_get_monitor_info(0, &info)) {
        return 1.0f;
    }

    // Alto disponible = alto del monitor menos el margen de la barra de tareas.
    // Ej: 1080 - 110 = 970 píxeles disponibles.
    const int availableHeight = (info.y2 - info.y1) - SCREEN_MARGIN;

    // Escala para que 1024 quepa en el alto disponible, sin agrandar nunca.
    // Ej: 970 / 1024 = 0.95; en un monitor gigante sería 1.0 (no se agranda).
    return std::min(1.0f, static_cast<float>(availableHeight) / SceneLayout::HEIGHT);
}

bool AllegroWindow::initialize(const std::string& title) {
    // Enciende Allegro. Siempre va primero, antes de cualquier otra función al_.
    // Ej: si falla, no tiene sentido seguir, por eso se retorna false.
    if (!al_init()) {
        return false;
    }

    // Activa el teclado para saber cuándo se presiona una tecla.
    // Ej: para cerrar el juego con la tecla ESC.
    al_install_keyboard();

    // Activa el módulo de imágenes.
    // Ej: para cargar assets/escenario.png como fondo.
    al_init_image_addon();

    // Trabaja desde la carpeta del proyecto para encontrar assets y los niveles.
    // Ej: funciona igual al ejecutar desde VS Code o con doble clic en Vista/main.exe.
    useProjectFolder();

    // Calcula qué tan grande se puede mostrar la ventana en esta pantalla.
    // Ej: en un portátil de 768 px de alto la escala queda cerca de 0.64.
    scale = computeScale();

    // Las imágenes que se carguen se verán suaves al reducirlas o agrandarlas.
    // Ej: sin esto, el fondo reducido al 70 % se vería pixelado.
    al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);

    // Crea la ventana con el tamaño lógico multiplicado por la escala.
    // Ej: con escala 0.9 -> 576 * 0.9 = 518 de ancho y 1024 * 0.9 = 921 de alto.
    display = al_create_display(static_cast<int>(SceneLayout::WIDTH * scale),
                                static_cast<int>(SceneLayout::HEIGHT * scale));

    // Si la ventana no se pudo crear, se avisa con false.
    // Ej: puede pasar si la tarjeta de video no soporta las opciones pedidas.
    if (display == nullptr) {
        return false;
    }

    // Pone el texto de la barra de título de la ventana.
    // Ej: "Bus Fever Party! FPOO-C++ Edition".
    al_set_window_title(display, title.c_str());

    // Crea la cola de eventos.
    // Ej: aquí llegarán las teclas, los clics y el aviso de cerrar la ventana.
    eventQueue = al_create_event_queue();

    // Crea el temporizador que avisa 60 veces por segundo.
    // Ej: 1.0 / 60.0 = 0.0166 segundos entre cada aviso.
    timer = al_create_timer(1.0 / FRAMES_PER_SECOND);

    // Si alguno de los dos falló, no se puede seguir.
    // Ej: el destructor se encargará de liberar lo que sí se creó.
    if (eventQueue == nullptr || timer == nullptr) {
        return false;
    }

    // Le dice a la cola: "anota lo que pase con la ventana" (como presionar la X).
    // Ej: genera ALLEGRO_EVENT_DISPLAY_CLOSE al cerrar.
    al_register_event_source(eventQueue, al_get_display_event_source(display));

    // Le dice a la cola: "anota lo que pase con el teclado".
    // Ej: genera ALLEGRO_EVENT_KEY_DOWN al presionar una tecla.
    al_register_event_source(eventQueue, al_get_keyboard_event_source());

    // Le dice a la cola: "anota cada tic del temporizador".
    // Ej: genera ALLEGRO_EVENT_TIMER 60 veces por segundo.
    al_register_event_source(eventQueue, al_get_timer_event_source(timer));

    // Transformación: una "regla de conversión" que se aplica a todo lo que se dibuje.
    // Ej: con escala 0.5, dibujar en (100, 100) aparece en (50, 50) de la ventana.
    ALLEGRO_TRANSFORM transform;

    // Empieza con la transformación neutra (no cambia nada).
    // Ej: es como multiplicar por 1.
    al_identity_transform(&transform);

    // Le agrega la escala en x y en y.
    // Ej: así se puede seguir dibujando con las medidas de 576 x 1024.
    al_scale_transform(&transform, scale, scale);

    // Activa la transformación para todo lo que se dibuje desde ahora.
    // Ej: el fondo de 576 x 1024 queda justo del tamaño de la ventana.
    al_use_transform(&transform);

    // Todo salió bien.
    // Ej: el main ya puede cargar la imagen de fondo.
    return true;
}

void AllegroWindow::startTimer() {
    // Pone a andar el reloj; desde aquí empiezan a llegar eventos de temporizador.
    // Ej: antes de esta línea, no llega ningún ALLEGRO_EVENT_TIMER.
    al_start_timer(timer);
}

void AllegroWindow::waitForEvent(ALLEGRO_EVENT& event) {
    // Espera a que llegue un evento a la cola y lo copia en "event".
    // Ej: si el usuario no hace nada, igual llegan los tics del temporizador.
    al_wait_for_event(eventQueue, &event);
}

bool AllegroWindow::isEventQueueEmpty() const {
    // Retorna true si no hay eventos pendientes por atender.
    // Ej: si llegaron 3 tics seguidos, se atienden antes de dibujar una sola vez.
    return al_is_event_queue_empty(eventQueue);
}

void AllegroWindow::present() {
    // Muestra de una vez todo lo dibujado.
    // Ej: Allegro dibuja primero "a escondidas" y aquí lo enseña completo.
    al_flip_display();
}

float AllegroWindow::getScale() const {
    // Retorna la escala actual de la ventana.
    // Ej: 0.9 si la ventana se muestra al 90 %.
    return scale;
}
