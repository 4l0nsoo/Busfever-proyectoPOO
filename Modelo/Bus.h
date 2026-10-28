/**
 * @file Bus.h
 * @brief Definición de la clase Bus
 *
 * @details
 * La implementacion de la abstraccion de Bus permite modelar los vehiculos, permitiendo
 * representar su identidad, ubicacion y desplazaineto dentro del tablero
 *
 * -----------------------------------------------------------------------------
 * Estándar CRC (Clase - Responsabilidad - Colaboración)
 * -----------------------------------------------------------------------------
 *
 * Clase:
 *   Bus
 *
 * Responsabilidades:
 *  - Almacena la identidad y las características del bus
 *  - Almacena la posicion y la direccion en la que avansara el objeto
 *  - Gestiona la disponibilidad de los asientos de los buses 
 *   
 * Colaboradores:
 *  - Traffic:
 *      * Contiene los buses y consulta posición, dirección y longitud
 *        para validar si un bus puede salir.
 *  - Parking:
 *      * Contiene los buses estacionados y los entrega para ser llenados.
 *  - WaitingRoom:
 *      * Sube pasajeros con board(n) y consulta isFull() y getSeatsLeft().
 *
 * -----------------------------------------------------------------------------
 *
 * @author Santiago Caicedo Cabrera, Gabriela Castaño Castillo, Alonso Javier Diaz Veliz, Jesus Rodrigo Arango Arias, Luis Felipe Mosquera Machado
 * @email santiago.caicedo.cabrera@correounivalle.edu.co, gabriela.castano@correounivalle.edu.co, alonso.diaz@correounivalle.edu.co, arango.jesus@correounivalle.edu.co
 *        

 */

#pragma once
#include <string>

class Bus {
private:
    int id;//identificador del bus
    int capacity;//cantidad de personas
    int color;
    std::string direction;//direccion en la que avanza
    int x;//posicion en x
    int y;//posicion en y
    int seatsLeft;

public:
    Bus(int id, int capacity, int color, const std::string& direction, int x, int y);

    void mover();
    int board(int n);
    bool isFull() const;
    int getLength() const;   // se calcula a partir de capacity (16 -> 1, 24 -> 2, 40 -> 3)

    int getLength() const;   // se calcula a partir de capacity (16 -> 1, 24 -> 2, 40 -> 3)
    
    // getters triviales (no van en el diagrama)
    int board(int n);
    bool isFull() const;
    int getId() const; 
    int getCapacity() const;
    int getColor() const;
    std::string getDirection() const;
    int getX() const;
    int getY() const;
    int getSeatsLeft() const; //Disponibilidad de asientos
};

/*board(n): resta n asientos. Aquí puedes protegerte, por ejemplo no restar más de los que quedan.
isFull(): devuelve true si seatsLeft == 0.
getSeatsLeft(): getter, para que la sala sepa cuánto cupo hay (implícito, no se dibuja).*/