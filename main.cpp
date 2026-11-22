#include "./Modelo/Traffic.h"
#include "./Modelo/Parking.h"
#include "./Modelo/Queue.h"
#include "./Modelo/WaitingRoom.h"
#include "./Controlador/Controller.h"
#include "./Vista/Console.h"
// Trae la clase que abre la ventana de Allegro, conectada con la consola.
// Ej: SceneryWindow sceneryWindow(...); sceneryWindow.run();
#include "./Vista/SceneryWindow.h"
// Trae el juego por consola (la misma lógica que antes estaba aquí en el main).
// Ej: se usa si la ventana de Allegro no se puede abrir.
#include "./Vista/ConsoleGame.h"

#include <exception>
#include <string>
// Trae std::system para ejecutar un comando de Windows.
// Ej: std::system("chcp 65001 > nul").
#include <cstdlib>

int main() {
    // Pone la consola de Windows en UTF-8 para que se vean bien las tildes.
    // Ej: sin esto, "Fácil" se vería como "FÃ¡cil".
    std::system("chcp 65001 > nul");

    // Modelo del juego: tablero, parqueadero, fila estática y fila rotatoria.
    // Ej: un solo modelo para la consola y para la ventana (es la MISMA partida).
    Traffic traffic;
    Parking parking;
    Queue queue;
    WaitingRoom waitingRoom(parking, queue);

    // Controller: la solución del juego (carga niveles y procesa los IDs de los buses).
    // Ej: controller.processCommand("7") manda el bus 7 al estacionamiento si puede salir.
    Controller controller(traffic, parking, waitingRoom, queue);

    // Consola: donde se juega escribiendo los IDs y se ve el tablero en texto.
    // Ej: console.printBoard(traffic).
    Console console;

    // Ventana de Allegro conectada a la consola: refleja con gráficos la partida de la consola.
    // Ej: al escribir "7" en la consola, el bus 7 sale del tablero también en la ventana.
    SceneryWindow sceneryWindow(controller, traffic, queue, waitingRoom, parking, console);

    // Abre la ventana y juega hasta que el usuario la cierre (con la X, ESC o "Salir").
    // Ej: run() retorna false si no se pudo abrir la ventana o faltan imágenes.
    if (!sceneryWindow.run()) {
        // Si la ventana no abre, se juega el mismo juego solo por consola.
        // Ej: en un computador sin Allegro o sin la carpeta assets.
        console.printError("No se pudo abrir la ventana de Allegro. Se juega solo por consola.");
        ConsoleGame consoleGame(controller, traffic, console);
        consoleGame.run();
    }

    return 0;
}
