#include "Passager.h"

Passager::Passager(int color, int groupAmount)
    : color(color), groupAmount(groupAmount) {}

Passager Passager::split(int n) {
    // TODO
    return Passager(color, 0);
}
