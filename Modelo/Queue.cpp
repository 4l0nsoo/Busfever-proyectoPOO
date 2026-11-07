#include "Queue.h"
#include <string>

void Queue::personQueue() {
    
}

void Queue::add(const Passager& passager) {
    queue.push_back(passager);
}

void Queue::clear() {
    queue.clear();
}

bool Queue::emptyFila() const {
    // Retorna true cuando ya no queda ningún grupo en la fila.
    // Ej: después de sacar el último grupo, emptyFila() da true.
    return queue.empty();
}

Passager Queue::removerFirstPassanger() {
    // Copia el primer grupo antes de quitarlo.
    // Ej: si el primero es rojo 16, first guarda rojo 16.
    // Antes de llamarlo hay que revisar que la fila no esté vacía con emptyFila().
    Passager first = queue.front();

    // Quita el primer grupo del vector (los demás avanzan un puesto).
    // Ej: el segundo grupo pasa a ser el primero (orden FIFO).
    queue.erase(queue.begin());

    // Retorna el grupo que salió.
    // Ej: WaitingRoom lo recibe para ponerlo en la fila rotatoria.
    return first;
}

Passager Queue::takeFront(int maxAmount) {
    // Si la fila está vacía o se piden 0 personas, retorna un grupo vacío.
    // Ej: takeFront(0) retorna un grupo de 0 personas y la fila no cambia.
    if (queue.empty() || maxAmount <= 0) {
        return Passager(0, 0);
    }

    // Si el primer grupo cabe completo, se saca entero.
    // Ej: primero azul 8 y maxAmount 10 -> sale azul 8 completo.
    if (queue.front().getGroupAmount() <= maxAmount) {
        return removerFirstPassanger();
    }

    // Si no cabe, se divide: salen maxAmount y el resto se queda de primero.
    // Ej: primero rojo 16 y maxAmount 10 -> sale rojo 10 y queda rojo 6.
    return queue.front().split(maxAmount);
}

const std::vector<Passager>& Queue::getPassengers() const {
    // Retorna la fila por referencia constante: se puede leer pero no cambiar.
    // Ej: la Vista sabe cuántas personas y de qué color hay sin copiar el vector.
    return queue;
}
