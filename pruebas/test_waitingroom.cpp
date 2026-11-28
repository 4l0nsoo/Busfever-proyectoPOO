/**
 * @file test_waitingroom.cpp
 * @brief Pruebas unitarias para la clase WaitingRoom.
 *
 * @details
 * Verifica el límite de capacidad máxima (CAPACITY = 120), transferencia de
 * pasajeros desde Queue, embarque parcial/completo a los buses compatibles,
 * despacho de buses llenos y reposición automática.
 *
 *-----------------------------------------------------------------------------
 * Compilación y ejecución:
 *   g++ pruebas/test_waitingroom.cpp Modelo/Passager.cpp Modelo/Queue.cpp Modelo/Bus.cpp Modelo/Parking.cpp Modelo/WaitingRoom.cpp -o pruebas/test_waitingroom.exe
 *   ./pruebas/test_waitingroom.exe
 * -----------------------------------------------------------------------------
 */

#include <iostream>
#include <cassert>
#include "../Modelo/Queue.h"
#include "../Modelo/Bus.h"
#include "../Modelo/Parking.h"
#include "../Modelo/WaitingRoom.h"

void testCapacidadMaximaFilaRotatoria() {
    Parking parking;
    Queue queue;
    WaitingRoom room(parking, queue);

    // Estado inicial vacío
    assert(room.bothRowsEmpty() == true);
    assert(room.getPeopleCount() == 0);

    // Entran 150 personas a la fila estática
    queue.add(Passager(1, 50));
    queue.add(Passager(2, 50));
    queue.add(Passager(3, 50));

    room.fillFromQueue();

    // En la sala deben entrar máximo 120 personas
    assert(room.getPeopleCount() == WaitingRoom::CAPACITY);
    assert(room.getPeopleCount() == 120);

    // Los grupos en la sala deben ser [50, 50, 20]
    assert(room.getSeats().size() == 3);
    assert(room.getSeats()[2].getGroupAmount() == 20);

    // En la cola estática deben quedar las 30 personas que no cupieron
    assert(queue.emptyFila() == false);
    assert(queue.getPassengers().size() == 1);
    assert(queue.getPassengers()[0].getGroupAmount() == 30);

    // Probar clear() de la sala
    room.clear();
    assert(room.getPeopleCount() == 0);
    assert(room.getSeats().empty() == true);
}

void testEmbarqueParcialYDespachoDeBusLleno() {
    Parking parking;
    Queue queue;
    WaitingRoom room(parking, queue);

    // 10 Rojos (1), 20 Azules (2), 15 Rojos (1) -> Total: 45 personas
    queue.add(Passager(1, 10));
    queue.add(Passager(2, 20));
    queue.add(Passager(1, 15));
    room.fillFromQueue();

    // Sin buses estacionados, nadie puede abordar
    assert(room.someoneCanBoard() == false);

    // Llega un bus Azul (color 2) de capacidad 16
    Bus busAzul(1, 16, 2, "derecha", 0, 0);
    parking.receiveBus(busAzul);
    assert(room.someoneCanBoard() == true);

    // Despachar pasajeros compatibles
    room.freePassager(parking);

    // El bus azul se llenó (16/16) y debió ser despachado del parqueadero
    assert(parking.getBuses().empty() == true);

    // De los 20 azules subieron 16 -> quedan 4 azules + 10 rojos + 15 rojos = 29
    assert(room.getPeopleCount() == 29);
    assert(room.getSeats()[1].getColor() == 2);
    assert(room.getSeats()[1].getGroupAmount() == 4);
}

void testEmbarqueCompletoYReposicionDesdeQueue() {
    Parking parking;
    Queue queue;
    WaitingRoom room(parking, queue);

    // Se llena la sala al tope (120 rojos) y  20 rojos esperando en Queue
    queue.add(Passager(1, 120));
    queue.add(Passager(1, 20));
    room.fillFromQueue();

    assert(room.getPeopleCount() == 120);
    assert(queue.getPassengers()[0].getGroupAmount() == 20);

    // Llega un bus Rojo (color 1) de capacidad 40
    Bus busRojo(2, 40, 1, "derecha", 0, 0);
    parking.receiveBus(busRojo);

    // Al ejecutar freePassager: suben 40 rojos al bus (se llena y se va), y automáticamente entran los 20 rojos que esperaban en Queue
    room.freePassager(parking);

    assert(parking.getBuses().empty() == true); // Bus despachado
    assert(queue.emptyFila() == true);          // Queue ya se vació
    assert(room.getPeopleCount() == 100);       // 120 - 40 + 20 = 100 personas
}

int main() {
    std::cout << "=== INICIANDO PRUEBAS UNITARIAS: CLASE WAITINGROOM ===\n";

    testCapacidadMaximaFilaRotatoria();
    std::cout << "  [OK] Capacidad maxima (120) y llenado desde Queue.\n";

    testEmbarqueParcialYDespachoDeBusLleno();
    std::cout << "  [OK] Embarque parcial, division de grupo y despacho de bus lleno.\n";

    testEmbarqueCompletoYReposicionDesdeQueue();
    std::cout << "  [OK] Reposicion automatica de pasajeros desde Queue tras embarque.\n";

    std::cout << "=== TODAS LAS PRUEBAS DE WAITINGROOM PASARON CORRECTAMENTE ===\n";
    return 0;
}