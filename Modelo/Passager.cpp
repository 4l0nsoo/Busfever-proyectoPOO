#include "Passager.h"

Passager::Passager(int color, int groupAmount)
    : color(color), groupAmount(groupAmount) {}

Passager Passager::split(int n) {
    if (n> groupAmount){
        groupAmount=n;
    }
    else{
        groupAmount-=n;
    }
    return Passager(color, n);
}
