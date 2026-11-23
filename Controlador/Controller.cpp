#include "Controller.h"
#include <string>
#include <stdexcept>

void Controller::loadLevel(const std::string& path) {
    // Le pide al cargador que lea el archivo y llene el tablero y la fila estática.
    // Ej: con "nivel1_facil.txt" quedan 34 buses en traffic y 42 grupos en queue.
    // La lectura del archivo ahora vive en TextLevelLoader (responsabilidad única).
    levelLoader.load(path, traffic, queue);

    // Vacía la fila rotatoria del nivel anterior.
    // Ej: al cambiar del nivel 1 al 2 no quedan pasajeros viejos en la glorieta.
    waitingRoom.clear();

    // Llena la fila rotatoria con los primeros pasajeros de la fila estática.
    // Ej: si caben 120 personas, pasan los primeros grupos hasta completar 120.
    waitingRoom.fillFromQueue();

    // Corrección: vacía el parqueadero del nivel anterior.
    // Ej: sin esto, al cargar otro nivel seguían los buses del nivel anterior y se llenaba enseguida.
    parking.clear();
}

void Controller::loadLevel(Difficulty difficulty) {
    // Carga el archivo que corresponde a la dificultad.
    // Ej: Difficulty::Normal -> loadLevel("nivel2_normal.txt").
    loadLevel(LevelCatalog::getFilePath(difficulty));

    // Recuerda la dificultad cargada (solo si la carga no lanzó excepción).
    // Ej: luego la ventana puede mostrar que se está en el nivel Normal.
    currentDifficulty = difficulty;
}

Difficulty Controller::getCurrentDifficulty() const {
    // Retorna la dificultad del último nivel cargado.
    // Ej: Difficulty::Easy al empezar el juego.
    return currentDifficulty;
}

bool Controller::processCommand(const std::string& command) {
    int id;
    std::size_t parsedCharacters = 0;

    // Convierte el comando a un ID y rechaza texto adicional.
    try {
        id = std::stoi(command, &parsedCharacters);
    } catch (const std::invalid_argument&) {
        return false;
    } catch (const std::out_of_range&) {
        return false;
    }

    if (parsedCharacters != command.size()) {
        return false;
    }

    // Busca el bus en el tráfico.
    Bus* bus = traffic.getBus(id);
    if (bus == nullptr) {
        return false;
    }

    if (parking.isFull() || !traffic.canExit(*bus)) {
        return false;
    }

    // Conserva una copia antes de quitar el bus del tráfico.
    Bus busLeaving = *bus;
    if (!traffic.removeBus(id)) {
        return false;
    }
    parking.receiveBus(busLeaving);
    return true;
}
