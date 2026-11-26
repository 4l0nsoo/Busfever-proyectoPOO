#include <iostream>
#include "tests.h"

int main() {
    probarBus();
    probarPassager();
    probarQueue();
    probarParking();
    probarTraffic();
    probarWaitingRoom();

    std::cout << "Todas las pruebas pasaron" << std::endl;
    return 0;
}
