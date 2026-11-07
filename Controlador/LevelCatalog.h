/**
 * @file LevelCatalog.h
 * @brief Definición de la clase LevelCatalog y del enum Difficulty
 *
 * @details
 * Relaciona cada nivel de dificultad (fácil, normal, difícil) con su archivo
 * plano .txt, su número y su nombre. Así el resto del programa pide un nivel por
 * su dificultad y no tiene que conocer los nombres de los archivos.
 *
 * -----------------------------------------------------------------------------
 * Estándar CRC (Clase - Responsabilidad - Colaboración)
 * -----------------------------------------------------------------------------
 *
 * Clase:
 *   LevelCatalog
 *
 * Responsabilidades:
 *  - Decir qué archivo .txt corresponde a cada dificultad.
 *  - Decir el número y el nombre de cada nivel (para mostrarlos).
 *  - Convertir un número (1, 2 o 3) en una dificultad.
 *
 * Colaboradores:
 *  - Controller:
 *      * Le pregunta la ruta del archivo para cargar el nivel pedido.
 *
 * Principio abierto/cerrado (OCP): para agregar un nivel nuevo solo se agrega
 * una fila a la tabla de niveles; las demás clases no cambian.
 *
 * -----------------------------------------------------------------------------
 *
 * @author Santiago Caicedo Cabrera, Gabriela Castaño Castillo, Alonso Javier Diaz Veliz, Jesus Rodrigo Arango Arias, Luis Felipe Mosquera Machado
 * @email santiago.caicedo.cabrera@correounivalle.edu.co, gabriela.castano@correounivalle.edu.co, alonso.diaz@correounivalle.edu.co, arango.jesus@correounivalle.edu.co
 */

#pragma once

// Trae std::string para las rutas y los nombres de los niveles.
// Ej: "nivel1_facil.txt".
#include <string>

// Niveles de dificultad del juego; el número es el número del nivel.
// Ej: Difficulty::Normal vale 2 y carga nivel2_normal.txt.
enum class Difficulty {
    Easy = 1,
    Normal = 2,
    Hard = 3
};

class LevelCatalog {
public:
    // Retorna la ruta del archivo .txt de la dificultad pedida.
    // Ej: LevelCatalog::getFilePath(Difficulty::Hard) da "nivel3_dificil.txt".
    static std::string getFilePath(Difficulty difficulty);

    // Retorna el nombre de la dificultad para mostrarlo al usuario.
    // Ej: LevelCatalog::getName(Difficulty::Easy) da "Fácil".
    static std::string getName(Difficulty difficulty);

    // Retorna el número del nivel.
    // Ej: LevelCatalog::getNumber(Difficulty::Normal) da 2.
    static int getNumber(Difficulty difficulty);

    // Convierte un número en dificultad; retorna false si el número no existe.
    // Ej: fromNumber(3, d) deja d = Difficulty::Hard y retorna true; fromNumber(7, d) retorna false.
    static bool fromNumber(int number, Difficulty& difficulty);
};
