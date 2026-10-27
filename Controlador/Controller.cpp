#include "Controller.h"
#include "../Vista/Console.h"
#include <string>

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


