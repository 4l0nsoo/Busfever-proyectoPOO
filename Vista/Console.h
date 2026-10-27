#pragma once
#include <string>
#include "../Modelo/Traffic.h"

class Console {
public:
    void printBoard(const Traffic& traffic) const;
    std::string readCommand() const;
    void printMessage(const std::string& message) const;
};
