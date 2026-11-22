#include "Console.h"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <vector>
// Trae _kbhit() y _getch() para leer el teclado de la consola sin detener el programa.
// Ej: _kbhit() dice si hay una tecla esperando; _getch() la lee sin esperar el Enter.
#include <conio.h>

void Console::printBoard(const Traffic& traffic) const {
    const std::vector<std::vector<int>>& grid = traffic.getGrid();
    if (grid.empty() || grid.front().empty()) {
        printMessage("El tablero esta vacio.");
        return;
    }

    const std::vector<Bus>& buses = traffic.getBuses();
    int maxBusId = 0;
    for (const Bus& bus : buses) {
        maxBusId = std::max(maxBusId, bus.getId());
    }

    const int cellWidth = std::max(
        3, static_cast<int>(std::to_string(maxBusId).size()) + 1);
    std::vector<char> directionById(maxBusId + 1, '?');
    for (const Bus& bus : buses) {
        const std::string direction = bus.getDirection();
        if (direction == "arriba") directionById[bus.getId()] = '^';
        else if (direction == "abajo") directionById[bus.getId()] = 'v';
        else if (direction == "izquierda") directionById[bus.getId()] = '<';
        else if (direction == "derecha") directionById[bus.getId()] = '>';
    }

    std::cout << "Tablero (ID y direccion; . = espacio libre)" << std::endl;
    std::cout << "Direccion: ^ arriba, v abajo, < izquierda, > derecha" << std::endl;
    std::cout << "    ";
    for (std::size_t x = 0; x < grid.front().size(); ++x) {
        std::cout << std::setw(cellWidth + 1) << x;
    }
    std::cout << std::endl;

    for (std::size_t row = grid.size(); row > 0; --row) {
        const std::size_t y = row - 1;
        std::cout << std::setw(3) << y << " |";
        for (int cell : grid[y]) {
            if (cell == 0) {
                std::cout << std::setw(cellWidth + 1) << ".";
            } else {
                // Corrección: las celdas vacías y los números del encabezado ocupan un espacio más,
                // así miden lo mismo que una celda con bus (número + flecha) y las columnas se alinean.
                // Ej: " 12<" y "   ." miden 4 caracteres cada una.
                const char direction = cell <= maxBusId ? directionById[cell] : '?';
                std::cout << std::setw(cellWidth) << std::to_string(cell) << direction;
            }
        }
        std::cout << std::endl;
    }
}

std::string Console::readCommand() const {
    std::string line;
    std::getline(std::cin, line);
    return line;
}

void Console::printMessage(const std::string& message) const {
    std::cout << message << std::endl;
}

void Console::printError(const std::string& error) const {
    std::cerr << "Error: " << error << std::endl;
}

void Console::printWin() const {
    printMessage("Ganaste.");
}

void Console::printLoss() const {
    printMessage("Perdiste.");
}

bool Console::tryReadCommand(std::string& command) {
    // Lee todas las teclas que estén esperando, sin detener el programa.
    // Ej: si el usuario escribió "1" y "2", se leen las dos.
    while (_kbhit()) {
        // Lee una tecla (sin esperar el Enter).
        // Ej: '1'.
        const int key = _getch();

        // Enter: el comando está completo.
        // Ej: "12" + Enter -> command = "12" y retorna true.
        if (key == '\r' || key == '\n') {
            std::cout << std::endl;
            command = pendingInput;
            pendingInput.clear();
            return true;
        }

        // Borrar (Backspace): quita la última letra escrita.
        // Ej: "12" -> "1".
        if (key == 8) {
            if (!pendingInput.empty()) {
                pendingInput.pop_back();
                std::cout << "\b \b" << std::flush;
            }
            continue;
        }

        // Teclas especiales (flechas, F1...) llegan como 0 o 224 seguidas de otro código: se ignoran.
        // Ej: flecha arriba.
        if (key == 0 || key == 224) {
            _getch();
            continue;
        }

        // Cualquier otra tecla se agrega al comando y se muestra en la consola.
        // Ej: '7' -> pendingInput = "7".
        pendingInput += static_cast<char>(key);
        std::cout << static_cast<char>(key) << std::flush;
    }

    // Todavía no se presionó Enter.
    // Ej: el usuario va escribiendo.
    return false;
}
