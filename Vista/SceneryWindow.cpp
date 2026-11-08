#include "SceneryWindow.h"
#include "Scenery.h"
#include "SpriteBank.h"
#include "PassengerRenderer.h"
#include "BoardRenderer.h"
#include "ParkingRenderer.h"
// Menús de la ventana: ahora están en la carpeta Vista/menus/.
// Ej: "menus/MainMenu.h" busca el archivo dentro de esa carpeta.
#include "menus/MainMenu.h"
#include "menus/PauseMenu.h"
#include "PauseButton.h"

// Trae std::exception para atrapar los errores al cargar un nivel.
// Ej: catch (const std::exception& error).
#include <exception>

// Trae std::vector para la lista de cosas que se dibujan.
// Ej: {fondo, personas, tablero, parqueadero}.
#include <vector>

// Constructor: guarda las referencias al Controller, al modelo y a la consola, y las rutas.
// Ej: SceneryWindow(controller, traffic, queue, waitingRoom, parking, console).
SceneryWindow::SceneryWindow(Controller& controller, const Traffic& traffic, const Queue& queue,
                             const WaitingRoom& waitingRoom, const Parking& parking, Console& console,
                             const std::string& title, const std::string& backgroundPath,
                             const std::string& spritesPath, const std::string& fontPath)
    : controller(controller), traffic(traffic), queue(queue), waitingRoom(waitingRoom),
      parking(parking), console(console), title(title), backgroundPath(backgroundPath),
      spritesPath(spritesPath), fontPath(fontPath) {}

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
        // Muestra en la consola por qué no se pudo cargar (igual que el juego de consola).
        // Ej: "No se pudo abrir el archivo del nivel: nivel2_normal.txt".
        console.printMessage(error.what());
        return false;
    }
}

bool SceneryWindow::run() {
    // Crea el objeto ventana (todavía no se ve nada).
    // Ej: se crea dentro de run() para que la ventana se cierre sola cuando run() termine.
    AllegroWindow window;

    // Enciende Allegro y abre la ventana; si falla, se avisa con false.
    // Ej: el main juega entonces solo por consola.
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

    // Los que dibujan a las personas, el tablero y el parqueadero.
    // Ej: los tres leen el modelo en cada cuadro, así reflejan lo que pasa en la consola.
    PassengerRenderer passengers(queue, waitingRoom, sprites);
    BoardRenderer board(traffic, sprites);
    ParkingRenderer parkingView(parking, sprites);

    // Capas de cada pantalla, en orden: lo primero queda al fondo.
    // Gracias al polimorfismo, el ciclo trata a todas igual (como IRenderable).
    // Ej: en pausa se dibuja el juego congelado y encima el menú de pausa.
    const std::vector<IRenderable*> menuLayers = {&scenery, &mainMenu};
    const std::vector<IRenderable*> playLayers = {&scenery, &passengers, &board, &parkingView, &pauseButton};
    const std::vector<IRenderable*> pausedLayers = {&scenery, &passengers, &board, &parkingView, &pauseMenu};

    // En pausa solo se actualiza el menú de pausa: el juego queda congelado.
    // Ej: las personas dejan de caminar.
    const std::vector<IRenderable*> pausedUpdates = {&pauseMenu};

    // Pantalla actual: se empieza en el menú principal.
    // Ej: pasa a Screen::Playing cuando se elige un nivel.
    Screen screen = Screen::Menu;

    // Indica si ya se ganó el nivel actual (ya no quedan buses en el tablero).
    // Ej: true después de sacar el último bus.
    bool levelWon = false;

    // Indica si la ventana sigue abierta y si toca volver a dibujar.
    // Ej: running pasa a false al cerrar la ventana o elegir "Salir".
    bool running = true;
    bool redraw = true;

    // Muestra en la consola las opciones del menú principal.
    // Ej: se llama al abrir la ventana y al volver al menú.
    // (Una lambda es una función pequeña escrita dentro de otra; [&] le deja usar las variables de run.)
    auto showMenuInConsole = [&]() {
        console.printMessage("");
        console.printMessage("=== Bus Fever Party! ===");
        console.printMessage("Elige un nivel en la ventana o escribe 1 (Facil), 2 (Normal) o 3 (Dificil). 'salir' para cerrar:");
    };

    // Un turno del juego de consola: imprime el tablero y pide el ID de un bus (igual que antes).
    // Ej: "Ingrese el ID de un bus o 'salir':".
    auto showTurnInConsole = [&]() {
        console.printBoard(traffic);
        console.printMessage("Ingrese el ID de un bus o 'salir':");
    };

    // Vuelve al menú principal (desde la consola o desde "Retirarse").
    // Ej: hay que elegir un nivel otra vez.
    auto goToMenu = [&]() {
        screen = Screen::Menu;
        window.setTitle(title);
        showMenuInConsole();
    };

    // Muestra el menú en la consola y arranca el reloj de 60 cuadros por segundo.
    // Ej: desde aquí llegan eventos ALLEGRO_EVENT_TIMER.
    showMenuInConsole();
    window.startTimer();

    // Ciclo principal: se repite mientras la ventana esté abierta.
    // Ej: cada vuelta atiende un evento y, si toca, redibuja.
    while (running) {
        // Espera (sin gastar procesador) a que llegue un evento.
        // Ej: una tecla, un clic, mover el mouse o un tic del reloj.
        ALLEGRO_EVENT event;
        window.waitForEvent(event);

        // Nivel que se pidió en este evento (0 = ninguno).
        // Ej: un clic en "Nivel 2", la tecla 2 o escribir "2" en la consola lo ponen en 2.
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
            if (screen == Screen::Menu) {
                mainMenu.setMousePosition(x, y);
            } else if (screen == Screen::Playing) {
                pauseButton.setMousePosition(x, y);
            } else {
                pauseMenu.setMousePosition(x, y);
            }
        }
        // Si hizo clic, se revisa qué tocó según la pantalla.
        // Ej: clic en "Nivel 1" carga el nivel fácil.
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
                    console.printMessage("Juego en pausa.");
                }
            }
            // En la pausa: "Continuar" vuelve al juego y "Retirarse" al menú principal.
            // Ej: después de retirarse hay que elegir un nivel otra vez.
            else {
                const MenuOption option = pauseMenu.optionAt(x, y);
                if (option == MenuOption::Resume) {
                    screen = Screen::Playing;
                    console.printMessage("Juego reanudado.");
                    showTurnInConsole();
                } else if (option == MenuOption::Retire) {
                    goToMenu();
                }
            }
            redraw = true;
        }
        // Si presionó una tecla en la ventana, se revisa cuál fue según la pantalla.
        // Ej: event.keyboard.keycode dice cuál tecla fue.
        else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            // Código de la tecla y si es una de las teclas de pausa (ESC o P).
            // Ej: ALLEGRO_KEY_P para la letra P.
            const int key = event.keyboard.keycode;
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
                    console.printMessage("Juego en pausa.");
                }
            }
            // En la pausa: ESC o P vuelven al juego.
            // Ej: igual que hacer clic en "Continuar".
            else if (pauseKey) {
                screen = Screen::Playing;
                console.printMessage("Juego reanudado.");
                showTurnInConsole();
            }
            redraw = true;
        }
        // Si fue un tic del reloj: se lee la consola (sin detener la ventana) y se avanzan las animaciones.
        // Ej: 60 veces por segundo.
        else if (event.type == ALLEGRO_EVENT_TIMER) {
            // Comando escrito en la consola (si el usuario ya presionó Enter).
            // Ej: "12".
            std::string command;
            if (console.tryReadCommand(command)) {
                // En el menú principal: "1", "2" o "3" eligen nivel y "salir" cierra.
                // Ej: "2" -> nivel normal.
                if (screen == Screen::Menu) {
                    Difficulty chosen;
                    if (command == "salir") {
                        running = false;
                    } else if (command.size() == 1 && LevelCatalog::fromNumber(command[0] - '0', chosen)) {
                        requestedLevel = command[0] - '0';
                    } else {
                        console.printError("Nivel invalido.");
                        showMenuInConsole();
                    }
                }
                // En la pausa: los comandos esperan hasta continuar.
                // Ej: "p" o "continuar" siguen; "salir" se retira al menú.
                else if (screen == Screen::Paused) {
                    if (command == "p" || command == "continuar") {
                        screen = Screen::Playing;
                        console.printMessage("Juego reanudado.");
                        showTurnInConsole();
                    } else if (command == "salir") {
                        goToMenu();
                    } else {
                        console.printMessage("El juego esta en pausa: escribe 'p' para continuar o 'salir' para retirarte.");
                    }
                }
                // En el juego: "salir" vuelve al menú (antes terminaba la partida).
                // Ej: el usuario escribe "salir".
                else if (command == "salir") {
                    goToMenu();
                }
                // Si ya se ganó, no quedan buses que mover.
                // Ej: se le recuerda cómo volver al menú.
                else if (levelWon) {
                    console.printMessage("Ya no quedan buses. Escribe 'salir' para volver al menu.");
                }
                // Lógica del juego de consola: el Controller procesa el ID y la consola muestra el resultado.
                // Ej: "2" -> "Bus enviado al estacionamiento." y la ventana mueve el bus 2 al parqueadero.
                else if (!command.empty()) {
                    if (controller.processCommand(command)) {
                        console.printMessage("Bus enviado al estacionamiento.");
                    } else {
                        console.printError("ID invalido, bus bloqueado o estacionamiento lleno. Intente de nuevo.");
                    }

                    // Si ya no quedan buses en el tablero, se ganó (misma condición que el juego de consola).
                    // Ej: después de sacar el último bus -> "Ganaste." en la consola y en el título de la ventana.
                    if (traffic.getBusCount() == 0) {
                        levelWon = true;
                        console.printWin();
                        console.printMessage("Escribe 'salir' para volver al menu.");
                        window.setTitle(title + " - ¡Ganaste!");
                    } else {
                        // Si no, se muestra el siguiente turno (tablero y pregunta), como en el ciclo de consola.
                        // Ej: el tablero ya sin el bus 2.
                        showTurnInConsole();
                    }
                }
                // Una línea vacía solo vuelve a mostrar el turno.
                // Ej: el usuario presionó Enter sin escribir nada.
                else {
                    showTurnInConsole();
                }
            }

            // Lo que se anima en cada pantalla (en pausa solo el menú de pausa).
            // Ej: en el juego, fondo, personas, tablero, parqueadero y botón de pausa.
            const std::vector<IRenderable*>& updates =
                (screen == Screen::Menu) ? menuLayers :
                (screen == Screen::Playing) ? playLayers : pausedUpdates;
            for (IRenderable* layer : updates) {
                layer->update(window.getFrameSeconds());
            }
            redraw = true;
        }

        // Si se pidió un nivel válido y cargó bien, empieza la partida.
        // Ej: clic en "Nivel 1" o escribir "1" -> se carga nivel1_facil.txt.
        Difficulty difficulty;
        if (LevelCatalog::fromNumber(requestedLevel, difficulty) && loadLevel(window, difficulty)) {
            // Crea los caminantes del nivel recién cargado.
            // Ej: 120 en la glorieta y los demás esperando en los caminos.
            passengers.syncWithModel();

            // Nombre del nivel para el menú de pausa y la consola.
            // Ej: "Nivel 2 (Normal)".
            const std::string levelName = "Nivel " + std::to_string(LevelCatalog::getNumber(difficulty)) +
                                          " (" + LevelCatalog::getName(difficulty) + ")";
            pauseMenu.setLevelName(levelName);
            screen = Screen::Playing;
            levelWon = false;
            redraw = true;

            // La consola empieza el juego de este nivel: título, tablero y pregunta.
            // Ej: "=== Nivel 1 (Fácil) ===" y el tablero con los 34 buses.
            console.printMessage("");
            console.printMessage("=== " + levelName + " ===");
            showTurnInConsole();
        }

        // Dibuja solo si toca y si no quedan eventos pendientes.
        // Ej: si llegaron 3 tics juntos, se dibuja una sola vez, no tres.
        if (redraw && window.isEventQueueEmpty()) {
            redraw = false;

            // Capas de la pantalla actual.
            // Ej: en pausa, el juego congelado y encima el menú de pausa.
            const std::vector<IRenderable*>& layers =
                (screen == Screen::Menu) ? menuLayers :
                (screen == Screen::Playing) ? playLayers : pausedLayers;

            // Dibuja cada capa en orden (cada una con su propio draw: polimorfismo).
            // Ej: fondo, personas, tablero, parqueadero y botón de pausa.
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
