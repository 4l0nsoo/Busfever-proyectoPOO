#pragma once
#include <string>
#include "../Modelo/Traffic.h"

class Console {
public:
    void printBoard(const Traffic& traffic) const;//imprime el tablero despues que traffic lo haya convertido en matriz
    std::string readCommand() const;
    void printMessage(const std::string& message) const;//auxiliar para imprimir en consola
    void printError(const std::string& error) const;//imprime el mensaje de error  
    void printWin() const;//imprime el mensaje de visctoria
    void printLoss() const;//imprime el mensaje de derrota
};
