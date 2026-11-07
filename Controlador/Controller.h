#pragma once
#include <string>
#include "../Modelo/Traffic.h"
#include "../Modelo/Parking.h"
#include "../Modelo/WaitingRoom.h"
#include "../Modelo/Queue.h"
// Interfaz para cargar niveles (el Controller depende de ella, no de una clase concreta).
// Ej: hoy se usa TextLevelLoader; mañana podría ser un cargador de JSON.
#include "ILevelLoader.h"
// Cargador de niveles desde archivos .txt que se usa por defecto.
// Ej: lee nivel1_facil.txt, nivel2_normal.txt y nivel3_dificil.txt.
#include "TextLevelLoader.h"
// Tabla que relaciona cada dificultad con su archivo .txt.
// Ej: Difficulty::Hard -> "nivel3_dificil.txt".
#include "LevelCatalog.h"

class Controller {
private:
    Traffic& traffic;
    Parking& parking;
    WaitingRoom& waitingRoom;
    Queue& queue;

    // Cargador de archivos .txt que se usa cuando no se entrega otro.
    // Ej: el constructor de 4 parámetros usa este.
    TextLevelLoader defaultLoader;

    // Cargador que realmente se usa (referencia a la interfaz).
    // Ej: apunta a defaultLoader o al cargador que se pase en el constructor.
    const ILevelLoader& levelLoader;

    // Dificultad del último nivel cargado.
    // Ej: después de loadLevel(Difficulty::Normal) vale Difficulty::Normal.
    Difficulty currentDifficulty;

public:
    Controller(Traffic& t, Parking& p, WaitingRoom& w, Queue& q)
        : traffic(t), parking(p), waitingRoom(w), queue(q),
          levelLoader(defaultLoader), currentDifficulty(Difficulty::Easy) {}

    // Constructor que recibe otro cargador de niveles (inversión de dependencias).
    // Ej: Controller controller(traffic, parking, waitingRoom, queue, cargadorJson);
    Controller(Traffic& t, Parking& p, WaitingRoom& w, Queue& q, const ILevelLoader& loader)
        : traffic(t), parking(p), waitingRoom(w), queue(q),
          levelLoader(loader), currentDifficulty(Difficulty::Easy) {}

    // Interpreta un comando como "mover bus 10 arriba" y ejecuta la acción.
    // Devuelve false si no se entendió el comando.
    bool processCommand(const std::string& command);// interpretar comando

    // Carga el nivel desde un archivo y llena la fila rotatoria; lanza excepción si falla.
    // Ej: controller.loadLevel("nivel1_facil.txt");
    void loadLevel(const std::string& path);

    // Carga el nivel de la dificultad pedida (busca su archivo en LevelCatalog).
    // Ej: controller.loadLevel(Difficulty::Hard); carga nivel3_dificil.txt.
    void loadLevel(Difficulty difficulty);

    // Retorna la dificultad del último nivel cargado.
    // Ej: sirve para mostrar "Nivel 2 - Normal" en la ventana.
    Difficulty getCurrentDifficulty() const;
};
