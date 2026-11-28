/**
 * @file test_parking.cpp
 * @brief Pruebas unitarias para la clase Parking.
 *
 * @details
 * Verifica la recepción de buses, el límite de capacidad de 4 puestos (isFull),
 * búsqueda de buses disponibles por color (findBus), despacho de buses llenos
 * (dispatchFullBuses) y limpieza del parqueadero (clear).
 *
 * -----------------------------------------------------------------------------
 * Compilación y ejecución:
 *   g++ pruebas/test_parking.cpp Modelo/Bus.cpp Modelo/Parking.cpp -o pruebas/test_parking.exe
 *   ./pruebas/test_parking.exe
 * -----------------------------------------------------------------------------
 */

#include <iostream>
#include <cassert>
#include "../Modelo/Bus.h"
#include "../Modelo/Parking.h"

void testRecepcionYCapacidadMaxima() {
    Parking parking;

    // Estado inicial vacío
    assert(parking.getBuses().empty() == true);
    assert(parking.isFull() == false);

    // Agregar 4 buses (capacidad máxima del parqueadero)
    Bus b1(1, 16, 1, "derecha", 0, 0);
    Bus b2(2, 24, 2, "arriba", 1, 0);
    Bus b3(3, 40, 3, "izquierda", 2, 0);
    Bus b4(4, 16, 1, "abajo", 3, 0);

    parking.receiveBus(b1);
    parking.receiveBus(b2);
    assert(parking.isFull() == false); // Van 2 de 4

    parking.receiveBus(b3);
    parking.receiveBus(b4);
    assert(parking.getBuses().size() == 4);
    assert(parking.isFull() == true); // Llegó al límite de 4 puestos
}

void testBusquedaPorColor() {
    Parking parking;

    Bus busRojo(10, 16, 1, "derecha", 0, 0);
    Bus busAzul(20, 24, 2, "derecha", 1, 0);
    parking.receiveBus(busRojo);
    parking.receiveBus(busAzul);

    // Buscar un bus azul (color 2) que tenga asientos libres
    Bus* encontrado = parking.findBus(2);
    assert(encontrado != nullptr);
    assert(encontrado->getId() == 20);

    // Buscar un color que no está estacionado (color 3)
    assert(parking.findBus(3) == nullptr);

    // Si el bus azul se llena por completo, findBus(2) ya no debe retornarlo
    encontrado->board(24);
    assert(encontrado->isFull() == true);
    assert(parking.findBus(2) == nullptr);
}

void testDespachoYLimpieza() {
    Parking parking;

    Bus b1(1, 16, 1, "derecha", 0, 0);
    Bus b2(2, 16, 2, "derecha", 1, 0);
    Bus b3(3, 24, 3, "derecha", 2, 0);

    // Se llena b1 y b3 antes de meterlos al parqueadero; b2 queda con cupos
    b1.board(16);
    b3.board(24);

    parking.receiveBus(b1);
    parking.receiveBus(b2);
    parking.receiveBus(b3);
    assert(parking.getBuses().size() == 3);

    // dispatchFullBuses debe sacar a b1 y b3, dejando solo a b2
    parking.dispatchFullBuses();
    assert(parking.getBuses().size() == 1);
    assert(parking.getBuses()[0].getId() == 2);

    // clear() debe vaciar por completo el parqueadero
    parking.clear();
    assert(parking.getBuses().empty() == true);
    assert(parking.isFull() == false);
}

int main() {
    std::cout << "=== INICIANDO PRUEBAS UNITARIAS: CLASE PARKING ===\n";

    testRecepcionYCapacidadMaxima();
    std::cout << "  [OK] Recepcion de buses y limite de 4 puestos (isFull).\n";

    testBusquedaPorColor();
    std::cout << "  [OK] Busqueda de buses con cupo por color (findBus).\n";

    testDespachoYLimpieza();
    std::cout << "  [OK] Despacho de buses llenos y limpieza (dispatchFullBuses / clear).\n";

    std::cout << "=== TODAS LAS PRUEBAS DE PARKING PASARON CORRECTAMENTE ===\n";
    return 0;
}