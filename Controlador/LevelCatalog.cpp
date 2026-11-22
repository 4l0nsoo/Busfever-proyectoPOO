#include "LevelCatalog.h"

// Espacio de nombres anónimo: lo de adentro solo se ve en este archivo.
// Ej: otro .cpp no puede usar LevelInfo por accidente.
namespace {

// Datos de un nivel: su dificultad, su archivo y su nombre.
// Ej: {Difficulty::Easy, "nivel1_facil.txt", "Fácil"}.
struct LevelInfo {
    Difficulty difficulty;
    const char* filePath;
    const char* name;
};

// Tabla con todos los niveles del juego, del más fácil al más difícil.
// Ej: para agregar un nivel 4 solo se agrega otra fila aquí (y el valor en el enum).
const LevelInfo LEVELS[] = {
    {Difficulty::Easy,   "nivel1_facil.txt",   "Fácil"},
    {Difficulty::Normal, "nivel2_normal.txt",  "Normal"},
    {Difficulty::Hard,   "nivel3_dificil.txt", "Difícil"}
};

// Busca en la tabla la fila de una dificultad.
// Ej: findLevel(Difficulty::Normal) retorna la fila de nivel2_normal.txt.
const LevelInfo& findLevel(Difficulty difficulty) {
    // Recorre las filas de la tabla una por una.
    // Ej: primero revisa Easy, luego Normal, luego Hard.
    for (const LevelInfo& level : LEVELS) {
        // Si la dificultad coincide, retorna esa fila.
        // Ej: Normal == Normal -> retorna la segunda fila.
        if (level.difficulty == difficulty) {
            return level;
        }
    }

    // Si no la encuentra (no debería pasar), retorna el nivel fácil.
    // Ej: así el juego siempre tiene un nivel que cargar.
    return LEVELS[0];
}

}

std::string LevelCatalog::getFilePath(Difficulty difficulty) {
    // Busca la fila y retorna su archivo.
    // Ej: Difficulty::Easy -> "nivel1_facil.txt".
    return findLevel(difficulty).filePath;
}

std::string LevelCatalog::getName(Difficulty difficulty) {
    // Busca la fila y retorna su nombre.
    // Ej: Difficulty::Hard -> "Difícil".
    return findLevel(difficulty).name;
}

int LevelCatalog::getNumber(Difficulty difficulty) {
    // Convierte el enum a número entero con static_cast.
    // Ej: Difficulty::Normal -> 2.
    return static_cast<int>(difficulty);
}

bool LevelCatalog::fromNumber(int number, Difficulty& difficulty) {
    // Recorre la tabla buscando un nivel con ese número.
    // Ej: number = 2 coincide con la fila de Difficulty::Normal.
    for (const LevelInfo& level : LEVELS) {
        // Compara el número pedido con el número de la fila.
        // Ej: getNumber(Difficulty::Hard) es 3.
        if (getNumber(level.difficulty) == number) {
            // Guarda la dificultad encontrada en el parámetro de salida.
            // Ej: quien llamó recibe difficulty = Difficulty::Normal.
            difficulty = level.difficulty;

            // Avisa que sí existe ese nivel.
            // Ej: fromNumber(2, d) retorna true.
            return true;
        }
    }

    // Ningún nivel tiene ese número.
    // Ej: fromNumber(9, d) retorna false y d no cambia.
    return false;
}
