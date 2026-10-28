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