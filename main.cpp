#include "./Modelo/Traffic.h"
#include "./Modelo/Parking.h"
#include "./Modelo/Queue.h"
#include "./Modelo/WaitingRoom.h"
#include "./Controlador/Controller.h"
#include "./Vista/Console.h"

#include <exception>
#include <string>

int main() {
    Traffic traffic;
    Parking parking;
    Queue queue;
    WaitingRoom waitingRoom(parking, queue);
    Controller controller(traffic, parking, waitingRoom, queue);
    Console console;

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
