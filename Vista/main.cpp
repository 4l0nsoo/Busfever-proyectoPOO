#include "./Modelo/Traffic.h"
#include "./Modelo/Parking.h"
#include "./Modelo/Queue.h"
#include "./Modelo/WaitingRoom.h"
#include "./Controlador/Controller.h"
#include "./Vista/Console.h"
// Trae la clase que abre la ventana de Allegro con el escenario.
// Ej: SceneryWindow sceneryWindow; sceneryWindow.run();
#include "./Vista/SceneryWindow.h"

#include <exception>
#include <string>

int main() {
    Traffic traffic;
    Parking parking;
    Queue queue;
    WaitingRoom waitingRoom(parking, queue);
    Controller controller(traffic, parking, waitingRoom, queue);
    Console console;

    // Crea la ventana de Allegro; recibe el controller (carga niveles) y el modelo (para dibujarlo).
    // Ej: primero muestra el menú para elegir el Nivel 1, 2 o 3.
    SceneryWindow sceneryWindow(controller, traffic, queue, waitingRoom);

    // Abre la ventana y espera a que el usuario la cierre (con la X o ESC).
    // Ej: si no encuentra el fondo o los sprites, avisa en la consola y el juego sigue.
    if (!sceneryWindow.run()) {
        console.printError("No se pudo abrir la ventana del escenario.");
    }

    try {
        controller.loadLevel("nivel1_facil.txt");
    } catch (const std::exception& error) {
        console.printMessage(error.what());
        return 1;
    }

    while (traffic.getBusCount() > 0) {
        console.printBoard(traffic);
        console.printMessage("Ingrese el ID de un bus o 'salir':");
        const std::string command = console.readCommand();

        if (command == "salir" || command.empty()) {
            break;
        }

        if (controller.processCommand(command)) {
            console.printMessage("Bus enviado al estacionamiento.");
        } else {
            console.printError("ID invalido, bus bloqueado o estacionamiento lleno. Intente de nuevo.");
        }
    }

    if (traffic.getBusCount() == 0) {
        console.printWin();
    }

    return 0;
}
