#include "Controller.h"
#include "../Vista/Console.h"
#include <string>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdexcept>

void Controller::loadLevel(const std::string& path) {
    Queue cola;
    std::ifstream file(path);

    if (!file.is_open()) {
        throw std::runtime_error("No se pudo abrir el archivo del nivel");
    }

    enum class Section {
        NONE,
        MATRIX,
        PASSENGERS,
        BUSES
    };

    Section section = Section::NONE;
    std::string line;

    while (std::getline(file, line)) {
        // Ignorar comentarios y líneas vacías.
        if (line.empty() || line[0] == '#') {
            continue;
        }

        std::istringstream iss(line);
        std::string first;

        iss >> first;

        // Detectar encabezados y cambiar de sección.
        if (first == "MATRIZ") {
            section = Section::MATRIX;
            continue;
        }

        if (first == "PASAJEROS") {
            section = Section::PASSENGERS;
            continue;
        }

        if (first == "BUSES") {
            section = Section::BUSES;
            continue;
        }

        // El tamaño del tablero.
        if (first == "TABLERO") {
            int rows, cols;
            iss >> rows >> cols;

            // Si Traffic necesita conocer las dimensiones,
            // configúralas aquí mediante su interfaz pública.
            continue;
        }

        iss.clear();
        iss.str(line);

        // Procesar los datos según la sección actual.
        switch (section) {
            case Section::MATRIX: {
                std::istringstream rowStream(line);
                int cell;

                while (rowStream >> cell) {
                    // Guardar la fila para reconstruir el grid,
                    // o utilizarla si Traffic permite cargar la matriz.
                }

                break;
            }

            case Section::PASSENGERS: {
                int color, quantity;

                if (iss >> color >> quantity) {
                    Passager passenger(color, quantity);
                    cola.add(passenger);
                }

                break;
            }

            case Section::BUSES: {
                int id, color, x, y, length, capacity;
                std::string direction;

                if (iss >> id >> color >> x >> y
                        >> direction >> length >> capacity) {

                    // length se lee para respetar el formato,
                    // pero Bus calcula su longitud desde capacity.
                    Bus bus(id, capacity, color, direction, x, y);
                    traffic.addBus(bus);
                }

                break;
            }

            case Section::NONE:
                break;
        }
    }
}

bool Controller::processCommand(const std::string& command) {
    int id;

    // Paso 1: convertir el texto a número
    try {
        id = std::stoi(command);
    } catch (...) {
        return false;   // no era un número ("abc", vacío...)
    }

    // Paso 2: buscar el bus
    Bus* bus = traffic.getBus(id);
    if (bus == nullptr) {
        return false;
    }

    // Paso 3: ¿puede salir?
    if (traffic.canExit(*bus)) {
        // TODO: sacar el bus (traffic, parking...)
        if(parking.isFull()){
                return true;
        }
    }

    return false;
}


