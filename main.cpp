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
    WaitingRoom waitingRoom;
    Controller controller(traffic, parking, waitingRoom);
    Console console;

    // TODO: cargar el nivel y armar el ciclo del juego
    console.printBoard(traffic);

    return 0;
}
