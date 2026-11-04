#include "Controller.h"
#include "../Vista/Console.h"
#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>

void Controller::loadLevel(const std::string& path) {
    std::ifstream file(path);

    if (!file.is_open()) {
        throw std::runtime_error("No se pudo abrir el archivo del nivel");
    }
    queue.clear();

    enum class Section {
        NONE,
        MATRIX,
        PASSENGERS,
        BUSES
    };

    Section section = Section::NONE;
    std::string line;
    int expectedPassengers = -1;
    int loadedPassengers = 0;
    int expectedBuses = -1;
    int loadedBuses = 0;

    while (std::getline(file, line)) {
        // Ignorar comentarios y líneas vacías.
        const std::size_t firstCharacter = line.find_first_not_of(" \t\r");
        if (firstCharacter == std::string::npos || line[firstCharacter] == '#') {
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
            if (!(iss >> expectedPassengers) || expectedPassengers < 0) {
                throw std::runtime_error("Cantidad de pasajeros invalida");
            }
            section = Section::PASSENGERS;
            continue;
        }

        if (first == "BUSES") {
            if (!(iss >> expectedBuses) || expectedBuses < 0) {
                throw std::runtime_error("Cantidad de buses invalida");
            }
            section = Section::BUSES;
            continue;
        }

        // El tamaño del tablero.
        if (first == "TABLERO") {
            int width, height;
            if (!(iss >> width >> height)) {
                throw std::runtime_error("Dimensiones del tablero invalidas");
            }
            traffic.initializeBoard(width, height);
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

                if (!(iss >> color >> quantity) || color <= 0 || quantity <= 0) {
                    throw std::runtime_error("Datos de pasajeros invalidos");
                }

                queue.add(Passager(color, quantity));
                ++loadedPassengers;
                break;
            }

            case Section::BUSES: {
                int id, color, x, y, length, capacity;
                std::string direction;

                if (!(iss >> id >> color >> x >> y
                        >> direction >> length >> capacity)) {
                    throw std::runtime_error("Datos de bus invalidos");
                }

                // length se lee para respetar el formato; Bus calcula su longitud desde capacity.
                Bus bus(id, capacity, color, direction, x, y);
                if (!traffic.addBus(bus)) {
                    throw std::runtime_error("No se pudo agregar el bus " + std::to_string(id));
                }

                ++loadedBuses;
                break;
            }

            case Section::NONE:
                break;
        }
    }

    if (expectedPassengers < 0 || loadedPassengers != expectedPassengers) {
        throw std::runtime_error("La cantidad de pasajeros no coincide con el nivel");
    }
    if (expectedBuses < 0 || loadedBuses != expectedBuses) {
        throw std::runtime_error("La cantidad de buses no coincide con el nivel");
    }
}

bool Controller::processCommand(const std::string& command) {
    int id;
    std::size_t parsedCharacters = 0;

    // Convierte el comando a un ID y rechaza texto adicional.
    try {
        id = std::stoi(command, &parsedCharacters);
    } catch (const std::invalid_argument&) {
        return false;
    } catch (const std::out_of_range&) {
        return false;
    }

    if (parsedCharacters != command.size()) {
        return false;
    }

    // Busca el bus en el tráfico.
    Bus* bus = traffic.getBus(id);
    if (bus == nullptr) {
        return false;
    }

    if (parking.isFull() || !traffic.canExit(*bus)) {
        return false;
    }

    // Conserva una copia antes de quitar el bus del tráfico.
    Bus busLeaving = *bus;
    if (!traffic.removeBus(id)) {
        return false;
    }
    parking.receiveBus(busLeaving);
    return true;
}
