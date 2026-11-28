/**
 * @file test_traffic.cpp
 * @brief Pruebas unitarias para la clase Traffic.
 *
 * @details
 * Verifica la inicialización del tablero, validación de dimensiones inválidas,
 * inserción de buses ocupando sus celdas correspondientes en el grid, prevención
 * de solapamientos y desbordes, verificación de salida libre (canExit) y
 * liberación de celdas al eliminar buses (removeBus).
 *
 * -----------------------------------------------------------------------------
 * Compilación y ejecución):
 *   g++ pruebas/test_traffic.cpp Modelo/Bus.cpp Modelo/Traffic.cpp -o pruebas/test_traffic.exe
 *   ./pruebas/test_traffic.exe
 * -----------------------------------------------------------------------------
 */

#include <iostream>
#include <cassert>
#include <stdexcept>
#include "../Modelo/Bus.h"
#include "../Modelo/Traffic.h"

void testInicializacionTablero() {
    // Tablero válido de 6x6
    Traffic traffic(6, 6);
    assert(traffic.getBusCount() == 0);
    assert(traffic.getGrid().size() == 6);
    assert(traffic.getGrid()[0].size() == 6);

    // Dimensiones inválidas deben dar std::invalid_argument
    bool lanzoExcepcion = false;
    try {
        Traffic invalido(0, 5);
    } catch (const std::invalid_argument&) {
        lanzoExcepcion = true;
    }
    assert(lanzoExcepcion == true);
}

void testInsercionYValidaciones() {
    Traffic traffic(6, 6);

    // Bus 1: capacidad 24 (ocupa 2 celdas) en (1, 2) mirando a la "derecha" -> ocupa (1,2) y (2,2)
    Bus b1(1, 24, 1, "derecha", 1, 2);
    assert(traffic.addBus(b1) == true);
    assert(traffic.getBusCount() == 1);
    assert(traffic.getGrid()[2][1] == 1);
    assert(traffic.getGrid()[2][2] == 1);

    // Error 1: ID duplicado
    Bus bDuplicado(1, 16, 2, "arriba", 0, 0);
    assert(traffic.addBus(bDuplicado) == false);

    // Error 2: Cruce con celdas ya ocupadas por Bus 1 en (2,2)
    Bus bChoca(2, 16, 2, "arriba", 2, 2);
    assert(traffic.addBus(bChoca) == false);

    // Error 3: Bus que se sale del borde del tablero (capacidad 40 = 3 celdas en x=4 -> 4, 5, 6 fuera de rango)
    Bus bFuera(3, 40, 3, "derecha", 4, 0);
    assert(traffic.addBus(bFuera) == false);

    // Error 4: Dirección inválida
    Bus bDirInvalida(4, 16, 1, "diagonal", 0, 0);
    assert(traffic.addBus(bDirInvalida) == false);
}

void testSalidaLibreYRemocion() {
    Traffic traffic(6, 6);

    // Escenario:
    // Bus 1 en (1, 1) longitud 2 (ocupa (1,1) y (2,1)) mirando a la "derecha"
    // Bus 2 en (4, 0) longitud 2 (ocupa (4,0) y (4,1)) mirando "arriba" -> BLOQUEA a Bus 1 en (4,1)
    Bus b1(1, 24, 1, "derecha", 1, 1);
    Bus b2(2, 24, 2, "arriba", 4, 0);

    assert(traffic.addBus(b1) == true);
    assert(traffic.addBus(b2) == true);

    // Bus 1 no puede salir porque Bus 2 está en su camino en (4,1)
    assert(traffic.canExit(b1) == false);

    // Bus 2 sí tiene el camino libre hacia arriba ((4,2), (4,3), (4,4), (4,5) están en 0)
    assert(traffic.canExit(b2) == true);

    // Remover Bus 2 del tablero (simula que salió hacia el parqueadero)
    assert(traffic.removeBus(2) == true);
    assert(traffic.getBusCount() == 1);
    assert(traffic.getBus(2) == nullptr);
    assert(traffic.getGrid()[1][4] == 0); // La celda (4,1) quedó libre

    // Ahora Bus 1 sí puede salir hacia la derecha
    assert(traffic.canExit(b1) == true);
}

int main() {
    std::cout << "=== INICIANDO PRUEBAS UNITARIAS: CLASE TRAFFIC ===\n";

    testInicializacionTablero();
    std::cout << "  [OK] Inicializacion del tablero y excepcion por dimensiones invalidas.\n";

    testInsercionYValidaciones();
    std::cout << "  [OK] Insercion de buses, ocupacion de celdas y control de colisiones.\n";

    testSalidaLibreYRemocion();
    std::cout << "  [OK] Verificacion de salida libre (canExit) y liberacion de celdas (removeBus).\n";

    std::cout << "=== TODAS LAS PRUEBAS DE TRAFFIC PASARON CORRECTAMENTE ===\n";
    return 0;
}