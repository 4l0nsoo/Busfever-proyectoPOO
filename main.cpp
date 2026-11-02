#include "./Modelo/Traffic.h"
#include "./Modelo/Parking.h"
#include "./Modelo/Queue.h"
#include "./Modelo/WaitingRoom.h"
#include "./Controlador/Controller.h"
#include "./Vista/Console.h"

int main() {
    Traffic traffic;
    Parking parking;
    Queue queue;
    WaitingRoom waitingRoom(parking, queue);
    Controller controller(traffic, parking, waitingRoom);
    Console console;

    // TODO: cargar el nivel y armar el ciclo del juego
        bool running = true;

    // while (running) {
    //     // a. Imprimir el estado actual del tablero
    //     console.printBoard(traffic);

    //     std::string comando = console.pedirComando();

    //     if (comando == "exit") {
    //         running = false;
    //         continue;
    //     }

    //     if (!controller.esComandoValido(comando)) {
    //         console.mostrarError("Comando no reconocido.");
    //         continue;
    //     }

    //     if (!controller.existeBus(traffic, comando)) {
    //         console.mostrarError("Bus no encontrado.");
    //         continue;
    //     }

    //     if (traffic.canExit(comando)) {[cite: 1, 2]
    //         // Extraemos el bus del grid (se quita de Traffic)
    //         Bus busLiberado = traffic.quitarBus(comando);[cite: 1]

    //         // Lo transferimos al vector del parqueadero
    //         parking.recibirBus(busLiberado);[cite: 1, 2]

    //         // WaitingRoom llena los buses usando la cola de pasajeros
    //         waitingRoom.freePassager();[cite: 2]
    //     } else {
    //         console.mostrarError("Bus bloqueado.");
    //     }
    //}

    return 0;
}
