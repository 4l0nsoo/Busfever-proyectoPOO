/**
 * @file test_queue.cpp
 * @brief Pruebas unitarias para la clase Queue.
 *
 * @details
 * Verifica el estado inicial de la cola, inserción de grupos de pasajeros,
 * extracción en orden FIFO, división parcial mediante takeFront() y limpieza.
 *
 * -----------------------------------------------------------------------------
 * Compilación y ejecución:
 *   g++ pruebas/test_queue.cpp Modelo/Passager.cpp Modelo/Queue.cpp -o pruebas/test_queue.exe
 *   ./pruebas/test_queue.exe
 * -----------------------------------------------------------------------------
 */

#include <iostream>
#include <cassert>
#include "../Modelo/Passager.h"
#include "../Modelo/Queue.h"

void testEstadoInicialYInsercion() {
    Queue q;
    assert(q.emptyFila() == true);
    assert(q.getPassengers().empty() == true);

    // Insertar dos grupos: Rojo (1) de 8 y Azul (2) de 16
    q.add(Passager(1, 8));
    q.add(Passager(2, 16));

    assert(q.emptyFila() == false);
    assert(q.getPassengers().size() == 2);
    assert(q.getPassengers()[0].getColor() == 1);
    assert(q.getPassengers()[0].getGroupAmount() == 8);
}

void testExtraccionFIFO() {
    Queue q;
    q.add(Passager(1, 8));
    q.add(Passager(2, 16));

    // Debe salir primero el grupo Rojo de 8
    Passager primero = q.removerFirstPassanger();
    assert(primero.getColor() == 1);
    assert(primero.getGroupAmount() == 8);

    // Ahora el frente debe ser el grupo Azul de 16
    assert(q.getPassengers().size() == 1);
    assert(q.getPassengers()[0].getColor() == 2);
    assert(q.getPassengers()[0].getGroupAmount() == 16);
}

void testTakeFrontYDivision() {
    Queue q;

    // Caso 1: takeFront en cola vacía retorna grupo (0, 0)
    Passager vacio = q.takeFront(5);
    assert(vacio.getColor() == 0);
    assert(vacio.getGroupAmount() == 0);

    q.add(Passager(2, 16));

    // Caso 2: pedir 0 personas no modifica la cola
    Passager cero = q.takeFront(0);
    assert(cero.getGroupAmount() == 0);
    assert(q.getPassengers()[0].getGroupAmount() == 16);

    // Caso 3: división: pedir 10 de un grupo de 16 -> salen 10, quedan 6
    Passager parcial = q.takeFront(10);
    assert(parcial.getColor() == 2);
    assert(parcial.getGroupAmount() == 10);
    assert(q.getPassengers().size() == 1);
    assert(q.getPassengers()[0].getGroupAmount() == 6);

    // Caso 4: pedir 10 cuando solo quedan 6 -> salen los 6 y cola queda vacía
    Passager resto = q.takeFront(10);
    assert(resto.getColor() == 2);
    assert(resto.getGroupAmount() == 6);
    assert(q.emptyFila() == true);
}

void testClear() {
    Queue q;
    q.add(Passager(1, 12));
    q.add(Passager(3, 24));
    assert(q.emptyFila() == false);

    q.clear();
    assert(q.emptyFila() == true);
    assert(q.getPassengers().empty() == true);
}

int main() {
    std::cout << "=== INICIANDO PRUEBAS UNITARIAS: CLASE QUEUE ===\n";

    testEstadoInicialYInsercion();
    std::cout << "  [OK] Insercion y estado inicial.\n";

    testExtraccionFIFO();
    std::cout << "  [OK] Extraccion en orden FIFO.\n";

    testTakeFrontYDivision();
    std::cout << "  [OK] Extraccion parcial y division de grupos (takeFront).\n";

    testClear();
    std::cout << "  [OK] Limpieza de la cola (clear).\n";

    std::cout << "=== TODAS LAS PRUEBAS DE QUEUE PASARON CORRECTAMENTE ===\n";
    return 0;
}