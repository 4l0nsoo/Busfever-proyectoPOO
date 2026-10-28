#include "Passager.h"

Passager::Passager(int color, int groupAmount) { //Pasajero constructor
    this->color = color;  // Parámetro color guardado en color
    this->groupAmount = groupAmount; // Cantidad de personas enviada por parámetro guardada en groupAmount
}

Passager Passager::split(int n) {

    // Caso 1: Si pide añadir al vehículo número incorrecto como 0 o negativo (Solicitud no válida)
    if (n <= 0) {
        return Passager(color, 0); // Devuelve un grupo vacío ya que nadie entró al bus
    }

    // Caso 2: Si se necesitan para subir al vehículo más o exactamente todas las personas del grupo
    if (n >= groupAmount) {
        int abordando = groupAmount; // Variable que tendrá la cantidad total de personas que enviaron por parámetro
        groupAmount = 0; // En el grupo no sobró ninguna persona
        return Passager(color, abordando); // Devuelve el grupo completo, todos se subieron
    }

    // Caso 3: Si solo se necesita una parte del grupo para subir al vehículo (Se divide en dos grupos)
    groupAmount -= n; // Al grupo se le resta las personas que se subieron al vehículo
    return Passager(color, n); // Devuelve un grupo con las personas que se subieron al vehículo
}
