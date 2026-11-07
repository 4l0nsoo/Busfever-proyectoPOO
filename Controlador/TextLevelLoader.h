/**
 * @file TextLevelLoader.h
 * @brief Definición de la clase TextLevelLoader
 *
 * @details
 * Carga un nivel desde un archivo plano .txt con las secciones TABLERO,
 * MATRIZ, PASAJEROS y BUSES. Crea el tablero en Traffic, agrega cada bus y
 * agrega cada grupo de pasajeros a la fila estática (Queue). Si el archivo
 * tiene un error, lanza una excepción con un mensaje que explica el problema.
 *
 * -----------------------------------------------------------------------------
 * Estándar CRC (Clase - Responsabilidad - Colaboración)
 * -----------------------------------------------------------------------------
 *
 * Clase:
 *   TextLevelLoader
 *
 * Responsabilidades:
 *  - Abrir y leer línea a línea el archivo del nivel.
 *  - Ignorar comentarios (#) y líneas vacías.
 *  - Crear el tablero con las dimensiones de TABLERO.
 *  - Crear los buses de la sección BUSES y agregarlos a Traffic.
 *  - Crear los grupos de PASAJEROS y agregarlos a Queue.
 *  - Validar que la cantidad de buses y pasajeros coincida con la declarada.
 *
 * Colaboradores:
 *  - ILevelLoader:
 *      * Es la interfaz que implementa.
 *  - Traffic, Bus:
 *      * Recibe el tablero y los buses del nivel.
 *  - Queue, Passager:
 *      * Recibe los grupos de pasajeros en el orden exacto del archivo.
 *
 * -----------------------------------------------------------------------------
 *
 * @author Santiago Caicedo Cabrera, Gabriela Castaño Castillo, Alonso Javier Diaz Veliz, Jesus Rodrigo Arango Arias, Luis Felipe Mosquera Machado
 * @email santiago.caicedo.cabrera@correounivalle.edu.co, gabriela.castano@correounivalle.edu.co, alonso.diaz@correounivalle.edu.co, arango.jesus@correounivalle.edu.co
 */

#pragma once
#include "ILevelLoader.h"

class TextLevelLoader : public ILevelLoader {
public:
    // Lee el archivo .txt y llena traffic y queue; lanza std::runtime_error si algo está mal.
    // Ej: loader.load("nivel2_normal.txt", traffic, queue); carga 71 buses y 95 grupos.
    void load(const std::string& path, Traffic& traffic, Queue& queue) const override;
};
