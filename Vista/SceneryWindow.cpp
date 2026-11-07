#include "SceneryWindow.h"
#include "Scenery.h"
#include "SpriteBank.h"
#include "PassengerRenderer.h"
#include "BoardRenderer.h"
#include "MainMenu.h"
#include "PauseMenu.h"
#include "PauseButton.h"

// Trae std::cerr para mostrar errores en la consola.
// Ej: std::cerr << "No se pudo cargar el nivel";
#include <iostream>

// Trae std::exception para atrapar los errores al cargar un nivel.
// Ej: catch (const std::exception& error).
#include <exception>

// Trae std::vector para las listas de cosas que se dibujan.
// Ej: {fondo, menú} o {fondo, personas, buses}.
#include <vector>

// Constructor: guarda las referencias al Controller y al modelo, y las rutas.
// Ej: SceneryWindow(controller, traffic, queue, waitingRoom) usa las rutas por defecto.
SceneryWindow::SceneryWindow(Controller& controller, const Traffic& traffic, const Queue& queue,
                             const WaitingRoom& waitingRoom, const std::string& title,
                             const std::string& backgroundPath, const std::string& spritesPath,
                             const std::string& fontPath)
    : controller(controller), traffic(traffic), queue(queue), waitingRoom(waitingRoom),
      title(title), backgroundPath(backgroundPath), spritesPath(spritesPath), fontPath(fontPath) {}

bool SceneryWindow::loadLevel(AllegroWindow& window, Difficulty difficulty) {
    // Intenta cargar el nivel; si el archivo tiene errores, se atrapa la excepción.
    // Ej: si falta nivel2_normal.txt, se muestra el error y se sigue en el menú.
    try {
        // Le pide al Controller (lógica) que cargue el nivel de esa dificultad.
        // Ej: Difficulty::Easy -> nivel1_facil.txt con 34 buses y 42 grupos de personas.
        controller.loadLevel(difficulty);

        // Muestra en la barra de título el nivel cargado.
        // Ej: "Bus Fever Party! FPOO-C++ Edition - Nivel 1 (Fácil)".
        window.setTitle(title + " - Nivel " + std::to_string(LevelCatalog::getNumber(difficulty)) +
                        " (" + LevelCatalog::getName(difficulty) + ")");
        return true;
    } catch (const std::exception& error) {
        // Muestra en la consola por qué no se pudo cargar.
        // Ej: "No se pudo cargar el nivel: No se pudo abrir el archivo del nivel: ...".
        std::cerr << "No se pudo cargar el nivel: " << error.what() << std::endl;
        return false;
    }
}

bool SceneryWindow::run() {
    // Crea el objeto ventana (todavía no se ve nada).
    // Ej: se crea dentro de run() para que la ventana se cierre sola cuando run() termine.
    AllegroWindow window;

    // Enciende Allegro y abre la ventana; si falla, se avisa con false.
    // Ej: el main puede mostrar un mensaje de error en la consola.
    if (!window.initialize(title)) {
        return false;
    }

    // Crea y carga el fondo; si no lo encuentra, se avisa con false.
    // Ej: falla si no existe assets/escenario.png.
    Scenery scenery;
    if (!scenery.loadBackground(backgroundPath)) {
        return false;
    }

    // Crea y carga el banco de sprites; si falta alguno, se avisa con false.
    // Ej: falla si se borró assets/sprites/personas/verde/caminar_2.png.
    SpriteBank sprites;
    if (!sprites.load(spritesPath)) {
        return false;
    }

    // Crea los dos menús (hijos de MenuPanel) y carga sus fuentes.
    // Ej: si no encuentra Arial, usan la letra de Allegro.
    MainMenu mainMenu;
    mainMenu.loadFonts(fontPath);
    PauseMenu pauseMenu;
    pauseMenu.loadFonts(fontPath);

    // Botón de pausa que se ve mientras se juega.
    // Ej: esquina de arriba a la derecha.
    PauseButton pauseButton;

    // Crea el que dibuja a las personas (con colisiones) y el que dibuja los buses.
    // Ej: los dos leen el modelo, que se llena al elegir un nivel.
    PassengerRenderer passengers(queue, waitingRoom, sprites);
    BoardRenderer board(traffic, sprites);

    // Capas de cada pantalla, en orden: lo primero queda al fondo.
    // Gracias al polimorfismo, el ciclo trata a todas igual (como IRenderable).
    // Ej: en pausa se dibuja el juego congelado y encima el menú de pausa.
    const std::vector<IRenderable*> menuLayers = {&scenery, &mainMenu};
    const std::vector<IRenderable*> playLayers = {&scenery, &passengers, &board, &pauseButton};
    const std::vector<IRenderable*> pausedLayers = {&scenery, &passengers, &board, &pauseMenu};

    // En pausa solo se actualiza el menú de pausa: el juego queda congelado.
    // Ej: las personas dejan de caminar y la glorieta deja de girar.
    const std::vector<IRenderable*> pausedUpdates = {&pauseMenu};

    // Pantalla actual: se empieza en el menú principal.
    // Ej: pasa a Screen::Playing cuando se elige un nivel.
    Screen screen = Screen::Menu;

    // Indica si la ventana sigue abierta.
    // Ej: se pone en false al cerrar la ventana o elegir "Salir".
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
        // Ej: puede ser una tecla, un clic, mover el mouse o un tic del reloj.
        ALLEGRO_EVENT event;

        // Espera (sin gastar procesador) a que llegue un evento.
        // Ej: si no pasa nada, llega el siguiente tic en 1/60 de segundo.
        window.waitForEvent(event);

        // Nivel que el usuario pidió en este evento (0 = ninguno).
        // Ej: un clic en "Nivel 2" o la tecla 2 en el menú lo ponen en 2.
        int requestedLevel = 0;

        // Si el usuario presionó la X de la ventana, se sale del ciclo.
        // Ej: ALLEGRO_EVENT_DISPLAY_CLOSE llega al hacer clic en la X.
        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            running = false;
        }
        // Si movió el mouse, se ilumina lo que esté debajo según la pantalla.
        // Ej: en pausa, el mouse sobre "Retirarse" le pone un borde blanco.
        else if (event.type == ALLEGRO_EVENT_MOUSE_AXES) {
            // Posición del mouse en medidas del diseño (576 x 1024).
            // Ej: con escala 0.77, x = 222 de la ventana es x = 288.
            const float x = window.toLogical(event.mouse.x);
            const float y = window.toLogical(event.mouse.y);

            // Le pasa la posición a lo que esté en pantalla.
            // Ej: en el juego, al botón de pausa.
            if (screen == Screen::Menu) {
                mainMenu.setMousePosition(x, y);
            } else if (screen == Screen::Playing) {
                pauseButton.setMousePosition(x, y);
            } else {
                pauseMenu.setMousePosition(x, y);
            }
        }
        // Si hizo clic, se revisa qué tocó según la pantalla.
        // Ej: clic en "Retirarse" vuelve al menú principal.
        else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            // Posición del clic en medidas del diseño.
            // Ej: (530, 46) es el botón de pausa.
            const float x = window.toLogical(event.mouse.x);
            const float y = window.toLogical(event.mouse.y);

            // En el menú principal: "Salir" cierra la ventana y un nivel queda pedido.
            // Ej: MenuOption::Level3 -> requestedLevel = 3.
            if (screen == Screen::Menu) {
                const MenuOption option = mainMenu.optionAt(x, y);
                if (option == MenuOption::Exit) {
                    running = false;
                } else if (option == MenuOption::Level1 || option == MenuOption::Level2 ||
                           option == MenuOption::Level3) {
                    requestedLevel = static_cast<int>(option);
                }
            }
            // En el juego: un clic en el botón de pausa abre el menú de pausa.
            // Ej: las personas se quedan quietas.
            else if (screen == Screen::Playing) {
                if (pauseButton.contains(x, y)) {
                    screen = Screen::Paused;
                }
            }
            // En la pausa: "Continuar" vuelve al juego y "Retirarse" al menú principal.
            // Ej: después de retirarse hay que elegir un nivel otra vez.
            else {
                const MenuOption option = pauseMenu.optionAt(x, y);
                if (option == MenuOption::Resume) {
                    screen = Screen::Playing;
                } else if (option == MenuOption::Retire) {
                    screen = Screen::Menu;
                    window.setTitle(title);
                }
            }
            redraw = true;
        }
        // Si presionó una tecla, se revisa cuál fue según la pantalla.
        // Ej: event.keyboard.keycode dice cuál tecla fue.
        else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            // Código de la tecla presionada.
            // Ej: ALLEGRO_KEY_P para la letra P.
            const int key = event.keyboard.keycode;

            // ESC o P sirven para pausar y para continuar.
            // Ej: ESC en el juego abre la pausa; ESC en la pausa la cierra.
            const bool pauseKey = (key == ALLEGRO_KEY_ESCAPE || key == ALLEGRO_KEY_P);

            // En el menú principal: 1, 2, 3 eligen nivel y ESC cierra la ventana.
            // Ej: tecla 3 -> requestedLevel = 3.
            if (screen == Screen::Menu) {
                if (key >= ALLEGRO_KEY_1 && key <= ALLEGRO_KEY_3) {
                    requestedLevel = key - ALLEGRO_KEY_0;
                } else if (key >= ALLEGRO_KEY_PAD_1 && key <= ALLEGRO_KEY_PAD_3) {
                    requestedLevel = key - ALLEGRO_KEY_PAD_0;
                } else if (key == ALLEGRO_KEY_ESCAPE) {
                    running = false;
                }
            }
            // En el juego: ESC o P abren el menú de pausa.
            // Ej: igual que hacer clic en el botón de pausa.
            else if (screen == Screen::Playing) {
                if (pauseKey) {
                    screen = Screen::Paused;
                }
            }
            // En la pausa: ESC o P vuelven al juego.
            // Ej: igual que hacer clic en "Continuar".
            else if (pauseKey) {
                screen = Screen::Playing;
            }
            redraw = true;
        }
        // Si fue un tic del reloj, se avanzan las animaciones de la pantalla actual.
        // Ej: 60 veces por segundo.
        else if (event.type == ALLEGRO_EVENT_TIMER) {
            // Lo que se actualiza en cada pantalla (en pausa, solo el menú de pausa).
            // Ej: en el juego, fondo, personas, buses y botón de pausa.
            const std::vector<IRenderable*>& updates =
                (screen == Screen::Menu) ? menuLayers :
                (screen == Screen::Playing) ? playLayers : pausedUpdates;

            // Avanza la animación (y la física de las personas) con el tiempo de un cuadro.
            // Ej: las personas caminan y se separan si se chocan.
            for (IRenderable* layer : updates) {
                layer->update(window.getFrameSeconds());
            }
            redraw = true;
        }

        // Dificultad que corresponde al nivel pedido.
        // Ej: 2 -> Difficulty::Normal.
        Difficulty difficulty;

        // Si se pidió un nivel válido y cargó bien, se pasa a la pantalla del juego.
        // Ej: clic en "Nivel 1" -> se carga nivel1_facil.txt y se ven los buses.
        if (LevelCatalog::fromNumber(requestedLevel, difficulty) && loadLevel(window, difficulty)) {
            // Crea los caminantes del nivel recién cargado.
            // Ej: 120 en la glorieta y los demás esperando en los caminos.
            passengers.syncWithModel();

            // Le dice al menú de pausa qué nivel se está jugando.
            // Ej: "Nivel 2 (Normal)".
            pauseMenu.setLevelName("Nivel " + std::to_string(LevelCatalog::getNumber(difficulty)) +
                                   " (" + LevelCatalog::getName(difficulty) + ")");
            screen = Screen::Playing;
            redraw = true;
        }

        // Dibuja solo si toca y si no quedan eventos pendientes.
        // Ej: si llegaron 3 tics juntos, se dibuja una sola vez, no tres.
        if (redraw && window.isEventQueueEmpty()) {
            // Ya se va a dibujar, así que se baja la bandera.
            // Ej: volverá a true con el siguiente tic.
            redraw = false;

            // Capas de la pantalla actual.
            // Ej: en pausa, el juego congelado y encima el menú de pausa.
            const std::vector<IRenderable*>& layers =
                (screen == Screen::Menu) ? menuLayers :
                (screen == Screen::Playing) ? playLayers : pausedLayers;

            // Dibuja cada capa en orden.
            // Ej: layer->draw() llama al draw() de cada clase (polimorfismo).
            for (const IRenderable* layer : layers) {
                layer->draw();
            }

            // Muestra en pantalla lo que se dibujó.
            // Ej: sin esta línea la ventana se queda en negro.
            window.present();
        }
    }

    // La ventana se cerró bien.
    // Ej: al salir de run(), los destructores liberan fuentes, sprites, fondo y ventana.
    return true;
}
