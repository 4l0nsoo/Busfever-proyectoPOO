#pragma once
#include <string>
#include "../Modelo/Traffic.h"
#include "../Modelo/Parking.h"
#include "../Modelo/WaitingRoom.h"
#include "../Modelo/Queue.h"

class Controller {
private:
    Traffic& traffic;
    Parking& parking;
    WaitingRoom& waitingRoom;

public:
    Controller(Traffic& traffic, Parking& parking, WaitingRoom& waitingRoom);

    // Interpreta un comando como "mover bus 10 arriba" y ejecuta la acción.
    // Devuelve false si no se entendió el comando.
    bool processCommand(const std::string& command);// interpretar comando 
    void loadLevel(const std::string& path);
};
