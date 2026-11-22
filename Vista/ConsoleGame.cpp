#include "ConsoleGame.h"

// Trae std::exception para atrapar los errores al cargar un nivel.
// Ej: catch (const std::exception& error).
#include <exception>
#include <string>

// Constructor: guarda las referencias al Controller, al tablero y a la consola.
// Ej: ConsoleGame(controller, traffic, console).
ConsoleGame::ConsoleGame(Controller& controller, const Traffic& traffic, Console& console)
    : controller(controller), traffic(traffic), console(console) {}

bool ConsoleGame::chooseLevel() {
    // Repite hasta que se cargue un nivel o el usuario escriba "salir".
    // Ej: si escribe "5", se le vuelve a preguntar.
    while (true) {
        // Pregunta el nivel.
        // Ej: el usuario escribe "1".
        console.printMessage("Elige un nivel: 1 (Facil), 2 (Normal) o 3 (Dificil). 'salir' para terminar:");
        const std::string command = console.readCommand();

        // "salir" o una línea vacía terminan el juego (igual que en el ciclo de turnos).
        // Ej: "salir" -> retorna false.
        if (command == "salir" || command.empty()) {
            return false;
        }

        // Convierte el texto en una dificultad; si no es 1, 2 o 3, se vuelve a preguntar.
        // Ej: "2" -> Difficulty::Normal.
        Difficulty difficulty;
        if (command.size() != 1 || !LevelCatalog::fromNumber(command[0] - '0', difficulty)) {
            console.printError("Nivel invalido.");
            continue;
        }

        // Le pide al Controller cargar el nivel; si el archivo falla, se muestra el error.
        // Ej: si falta nivel3_dificil.txt.
        try {
            controller.loadLevel(difficulty);
            return true;
        } catch (const std::exception& error) {
            console.printMessage(error.what());
        }
    }
}

void ConsoleGame::run() {
    // Primero se elige el nivel; si el usuario sale, no se juega.
    // Ej: "salir" -> termina.
    if (!chooseLevel()) {
        return;
    }

    // Turnos del juego: misma lógica que tenía el main, mientras queden buses en el tablero.
    // Ej: con 34 buses se repite hasta sacarlos todos o escribir "salir".
    while (traffic.getBusCount() > 0) {
        // Imprime el tablero y pide el ID de un bus.
        // Ej: el usuario escribe "2".
        console.printBoard(traffic);
        console.printMessage("Ingrese el ID de un bus o 'salir':");
        const std::string command = console.readCommand();

        // "salir" o una línea vacía terminan la partida.
        // Ej: el usuario escribe "salir".
        if (command == "salir" || command.empty()) {
            break;
        }

        // Le pide al Controller procesar el comando y muestra el resultado.
        // Ej: si el bus tenía la salida libre -> "Bus enviado al estacionamiento.".
        if (controller.processCommand(command)) {
            console.printMessage("Bus enviado al estacionamiento.");
        } else {
            console.printError("ID invalido, bus bloqueado o estacionamiento lleno. Intente de nuevo.");
        }
    }

    // Si ya no quedan buses en el tablero, se ganó.
    // Ej: "Ganaste.".
    if (traffic.getBusCount() == 0) {
        console.printWin();
    }
}
