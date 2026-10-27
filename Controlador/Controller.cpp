#include "Controller.h"

Controller::Controller(Traffic& traffic, Parking& parking, WaitingRoom& waitingRoom)
    : traffic(traffic), parking(parking), waitingRoom(waitingRoom) {}

bool Controller::processCommand(const std::string& command) {
    // TODO
    return false;
}
