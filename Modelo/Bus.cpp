#include "Bus.h"

Bus::Bus(int id, int capacity, int color, const std::string& direction, int x, int y)
:id(id), capacity(capacity), color(color), direction(direction), x(x), y(y), seatsLeft(capacity) {}

int Bus::getLength() const{
    if(capacity==16){return 1;}
    if(capacity==24){return 2;}
    if(capacity==40){return 3;}
    return 0;
}

void Bus::mover() {
    if((direction=="arriba") || (direction=="v")){
        y+=1;
    }else if(direction=="abajo"){
        y-=1;
    }else if((direction=="derecha") || (direction=="H")){
        x+=1;
    }else if(direction=="izquierda"){
        x-=1;
    }
    // TODO
}

int Bus::getId() const { return id; }
int Bus::getCapacity() const { return capacity; }
int Bus::getColor() const { return color; }
std::string Bus::getDirection() const { return direction; }
int Bus::getX() const { return x; }
int Bus::getY() const { return y; }
int Bus::getSeatsLeft() const {return seatsLeft;}

bool Bus::isFull() const{
        return seatsLeft==0;
    }

int Bus::board(int n){
    if(n<=seatsLeft){
        seatsLeft-=n;
        return 0;
    }
    int sobrantes= n-seatsLeft;
    seatsLeft=0;
    return sobrantes; 
}