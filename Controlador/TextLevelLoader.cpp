#include "TextLevelLoader.h"

// Trae std::ifstream para leer archivos.
// Ej: std::ifstream file("nivel1_facil.txt");
#include <fstream>

// Trae std::istringstream para separar una línea en palabras y números.
// Ej: "BUSES 34" -> palabra "BUSES" y número 34.
#include <sstream>

// Trae std::runtime_error para avisar errores del archivo.
// Ej: throw std::runtime_error("Datos de bus invalidos");
#include <stdexcept>

void TextLevelLoader::load(const std::string& path, Traffic& traffic, Queue& queue) const {
    // Abre el archivo del nivel.
    // Ej: path = "nivel1_facil.txt".
    std::ifstream file(path);

    // Si no se pudo abrir, avisa con una excepción.
    // Ej: pasa si el archivo no existe o tiene otro nombre.
    if (!file.is_open()) {
        throw std::runtime_error("No se pudo abrir el archivo del nivel: " + path);
    }

    // Vacía la fila de pasajeros del nivel anterior.
    // Ej: al pasar del nivel 1 al 2 no quedan pasajeros viejos.
    queue.clear();

    // Secciones posibles del archivo.
    // Ej: después de la línea "BUSES 34" se está en la sección BUSES.
    enum class Section {
        NONE,
        MATRIX,
        PASSENGERS,
        BUSES
    };

    // Sección en la que va la lectura (al inicio ninguna).
    // Ej: cambia a PASSENGERS al leer "PASAJEROS 42".
    Section section = Section::NONE;

    // Guarda cada línea leída del archivo.
    // Ej: "1 1 0 6 arriba 2 24".
    std::string line;

    // Cantidad de grupos de pasajeros que dice el archivo (-1 = todavía no se sabe).
    // Ej: "PASAJEROS 42" -> 42.
    int expectedPassengers = -1;

    // Cantidad de grupos de pasajeros leídos de verdad.
    // Ej: al final debe ser igual a expectedPassengers.
    int loadedPassengers = 0;

    // Cantidad de buses que dice el archivo (-1 = todavía no se sabe).
    // Ej: "BUSES 34" -> 34.
    int expectedBuses = -1;

    // Cantidad de buses leídos de verdad.
    // Ej: al final debe ser igual a expectedBuses.
    int loadedBuses = 0;

    // Lee el archivo línea por línea hasta el final.
    // Ej: getline devuelve false cuando ya no hay más líneas.
    while (std::getline(file, line)) {
        // Busca el primer carácter que no sea espacio, tabulación o fin de línea.
        // Ej: en "   # comentario" lo encuentra en la posición 3.
        const std::size_t firstCharacter = line.find_first_not_of(" \t\r");

        // Ignora las líneas vacías y los comentarios que empiezan con #.
        // Ej: "# Colores: 1=rojo 2=azul" se salta.
        if (firstCharacter == std::string::npos || line[firstCharacter] == '#') {
            continue;
        }

        // Prepara la línea para leer sus partes una por una.
        // Ej: "TABLERO 8 8" -> "TABLERO", 8, 8.
        std::istringstream iss(line);

        // Primera palabra de la línea.
        // Ej: "BUSES", "PASAJEROS" o un número.
        std::string first;

        // Lee la primera palabra.
        // Ej: de "PASAJEROS 42" lee "PASAJEROS".
        iss >> first;

        // Encabezado MATRIZ: empieza la sección de la matriz.
        // Ej: las filas siguientes muestran los ids de los buses por celda.
        if (first == "MATRIZ") {
            section = Section::MATRIX;
            continue;
        }

        // Encabezado PASAJEROS: lee cuántos grupos vienen.
        // Ej: "PASAJEROS 42" -> expectedPassengers = 42.
        if (first == "PASAJEROS") {
            // Si el número falta o es negativo, el archivo está mal.
            // Ej: "PASAJEROS -3" lanza el error.
            if (!(iss >> expectedPassengers) || expectedPassengers < 0) {
                throw std::runtime_error("Cantidad de pasajeros invalida");
            }
            section = Section::PASSENGERS;
            continue;
        }

        // Encabezado BUSES: lee cuántos buses vienen.
        // Ej: "BUSES 34" -> expectedBuses = 34.
        if (first == "BUSES") {
            // Si el número falta o es negativo, el archivo está mal.
            // Ej: "BUSES" sin número lanza el error.
            if (!(iss >> expectedBuses) || expectedBuses < 0) {
                throw std::runtime_error("Cantidad de buses invalida");
            }
            section = Section::BUSES;
            continue;
        }

        // Línea TABLERO: ancho y alto del tablero.
        // Ej: "TABLERO 8 8" crea un tablero de 8 x 8 celdas.
        if (first == "TABLERO") {
            // Variables para el ancho y el alto.
            // Ej: width = 12, height = 12 en el nivel normal.
            int width, height;

            // Si faltan números, el archivo está mal.
            // Ej: "TABLERO 8" lanza el error.
            if (!(iss >> width >> height)) {
                throw std::runtime_error("Dimensiones del tablero invalidas");
            }

            // Crea el tablero vacío (también borra los buses anteriores).
            // Ej: traffic queda con 8 x 8 celdas en 0.
            traffic.initializeBoard(width, height);
            continue;
        }

        // Vuelve a leer la línea desde el principio para procesar sus datos.
        // Ej: "2 8" se lee otra vez completa como color y cantidad.
        iss.clear();
        iss.str(line);

        // Procesa la línea según la sección actual.
        // Ej: en PASSENGERS, "2 8" es un grupo de 8 personas azules.
        switch (section) {
            case Section::MATRIX:
                // La matriz solo sirve para que una persona lea el archivo;
                // el tablero se arma con la sección BUSES.
                // Ej: la fila " 1  2  3  3" se ignora.
                break;

            case Section::PASSENGERS: {
                // Color y cantidad de personas del grupo.
                // Ej: "5 16" -> color 5 (rosado), 16 personas.
                int color, quantity;

                // Si faltan datos o son cero/negativos, el archivo está mal.
                // Ej: "3 0" lanza el error porque un grupo no puede estar vacío.
                if (!(iss >> color >> quantity) || color <= 0 || quantity <= 0) {
                    throw std::runtime_error("Datos de pasajeros invalidos");
                }

                // Agrega el grupo al final de la fila estática.
                // Ej: el primer grupo del archivo queda primero en la fila.
                queue.add(Passager(color, quantity));

                // Cuenta un grupo más leído.
                // Ej: después de 42 grupos, loadedPassengers = 42.
                ++loadedPassengers;
                break;
            }

            case Section::BUSES: {
                // Datos del bus en el orden del archivo.
                // Ej: "1 1 0 6 arriba 2 24" -> id 1, color 1, x 0, y 6, largo 2, capacidad 24.
                int id, color, x, y, length, capacity;

                // Dirección hacia la que sale el bus.
                // Ej: "arriba", "abajo", "izquierda" o "derecha".
                std::string direction;

                // Si falta algún dato, el archivo está mal.
                // Ej: "1 1 0 6 arriba" (sin largo ni capacidad) lanza el error.
                if (!(iss >> id >> color >> x >> y >> direction >> length >> capacity)) {
                    throw std::runtime_error("Datos de bus invalidos");
                }

                // Crea el bus; su largo lo calcula Bus a partir de la capacidad.
                // Ej: capacidad 40 -> bus grande de 3 celdas.
                Bus bus(id, capacity, color, direction, x, y);

                // Lo agrega al tablero; si se sale o choca con otro bus, es un error.
                // Ej: un bus en x = 9 en un tablero de 8 columnas no cabe.
                if (!traffic.addBus(bus)) {
                    throw std::runtime_error("No se pudo agregar el bus " + std::to_string(id));
                }

                // Cuenta un bus más leído.
                // Ej: después de 34 buses, loadedBuses = 34.
                ++loadedBuses;
                break;
            }

            case Section::NONE:
                // Línea antes de cualquier encabezado: no se usa.
                // Ej: texto suelto al principio del archivo.
                break;
        }
    }

    // Revisa que se leyeran exactamente los grupos de pasajeros anunciados.
    // Ej: si dice "PASAJEROS 42" pero hay 41 líneas, lanza el error.
    if (expectedPassengers < 0 || loadedPassengers != expectedPassengers) {
        throw std::runtime_error("La cantidad de pasajeros no coincide con el nivel");
    }

    // Revisa que se leyeran exactamente los buses anunciados.
    // Ej: si dice "BUSES 34" pero hay 35 líneas, lanza el error.
    if (expectedBuses < 0 || loadedBuses != expectedBuses) {
        throw std::runtime_error("La cantidad de buses no coincide con el nivel");
    }
}
