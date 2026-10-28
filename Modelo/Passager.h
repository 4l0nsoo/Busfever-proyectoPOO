/**
 * @file Passager.h
 * @brief Definición de la clase Passager 
 *
 * @details
 * Representa a un grupo de personas de un mismo color dentro del juego Bus Fever Party.
 * Almacena el código numérico del color y la cantidad actual de integrantes. Su función 
 * principal es permitir la división del grupo mediante el método split(), adaptando el 
 * número de pasajeros a la capacidad disponible del vehículo que va a abordar.
 *
 * -----------------------------------------------------------------------------
 * Estándar CRC (Clase - Responsabilidad - Colaboración)
 * -----------------------------------------------------------------------------
 *
 * Clase:
 *   Passager 
 *
 * Responsabilidades:
 *  - Almacenar el color que identifica al grupo de personas.
 *  - Registrar la cantidad de personas que conforman el grupo.
 *  - Dividir el grupo (split) cuando una parte o la totalidad aborda un vehículo.
 *  - Proveer acceso de lectura a sus atributos privados (getColor, getGroupAmount).
 *  
 * Colaboradores:
 *   - Queue:
 *       * Contiene la lista estática de objetos Passager al inicio del nivel.
 *   - WaitingRoom:
 *       * Recibe objetos Passager desde Queue para gestionar el turno de embarque.
 *
 * -----------------------------------------------------------------------------
 *
 * @author Santiago Caicedo Cabrera, Gabriela Castaño Castillo, Alonso Javier Diaz Veliz, Jesus Rodrigo Arango Arias, Luis Felipe Mosquera Machado
 * @email santiago.caicedo.cabrera@correounivalle.edu.co, gabriela.castano@correounivalle.edu.co, alonso.diaz@correounivalle.edu.co, arango.jesus@correounivalle.edu.co
 *        
 */

#pragma once

class Passager {
private:
    int color; // Color del pasajero
    int groupAmount;// Cantidad de personas en un grupo

public:
    Passager(int color, int groupAmount);

    // Parte el grupo: resta n a groupAmount y devuelve un grupo nuevo de n
    Passager split(int n);

    // Getters para color y groupAmount porque son atributos privados
    int getColor() const { return color; }
    int getGroupAmount() const { return groupAmount; }
};