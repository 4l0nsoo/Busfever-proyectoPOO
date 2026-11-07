/**
 * @file ILevelLoader.h
 * @brief Definición de la interfaz ILevelLoader
 *
 * @details
 * Contrato que debe cumplir cualquier clase que sepa cargar un nivel: leer la
 * fuente del nivel y llenar el tablero (Traffic) con los buses y la fila
 * estática (Queue) con los pasajeros.
 *
 * -----------------------------------------------------------------------------
 * Estándar CRC (Clase - Responsabilidad - Colaboración)
 * -----------------------------------------------------------------------------
 *
 * Clase:
 *   ILevelLoader (interfaz, solo métodos virtuales puros)
 *
 * Responsabilidades:
 *  - Definir el método load() que llena Traffic y Queue desde una fuente.
 *
 * Colaboradores:
 *  - TextLevelLoader:
 *      * Implementa la interfaz leyendo archivos planos .txt.
 *  - Controller:
 *      * Depende de esta interfaz y no de una clase concreta.
 *
 * Principio de inversión de dependencias (DIP): Controller usa ILevelLoader, así
 * se puede cambiar la forma de cargar niveles (por ejemplo, desde JSON) sin
 * modificar el Controller.
 *
 * -----------------------------------------------------------------------------
 *
 * @author Santiago Caicedo Cabrera, Gabriela Castaño Castillo, Alonso Javier Diaz Veliz, Jesus Rodrigo Arango Arias, Luis Felipe Mosquera Machado
 * @email santiago.caicedo.cabrera@correounivalle.edu.co, gabriela.castano@correounivalle.edu.co, alonso.diaz@correounivalle.edu.co, arango.jesus@correounivalle.edu.co
 */

#pragma once
#include <string>
#include "../Modelo/Traffic.h"
#include "../Modelo/Queue.h"

class ILevelLoader {
public:
    // Destructor virtual: permite destruir bien una clase hija desde un puntero a la interfaz.
    // Ej: delete loader; llama también al destructor de TextLevelLoader.
    virtual ~ILevelLoader() = default;

    // Llena traffic con los buses y queue con los pasajeros del nivel; lanza excepción si falla.
    // Ej: loader.load("nivel1_facil.txt", traffic, queue);
    virtual void load(const std::string& path, Traffic& traffic, Queue& queue) const = 0;
};
