#include "Console.h"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <vector>

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
        std::cout << std::setw(cellWidth) << x;
    }
    std::cout << std::endl;

    for (std::size_t row = grid.size(); row > 0; --row) {
        const std::size_t y = row - 1;
        std::cout << std::setw(3) << y << " |";
        for (int cell : grid[y]) {
            if (cell == 0) {
                std::cout << std::setw(cellWidth) << ".";
            } else {
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
