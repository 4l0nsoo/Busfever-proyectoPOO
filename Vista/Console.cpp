#include "Console.h"
#include <iostream>

void Console::printBoard(const Traffic& traffic) const {
    // TODO: imprimir el tablero en ASCII
}

std::string Console::readCommand() const {
    std::string line;
    std::getline(std::cin, line);
    return line;
}

void Console::printMessage(const std::string& message) const {
    std::cout << message << std::endl;
}
