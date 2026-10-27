#pragma once

class Passager {
private:
    int color;
    int groupAmount;//cantidad de personas en un grupo

public:
    Passager(int color, int groupAmount);

    // Parte el grupo: resta n a groupAmount y devuelve un grupo nuevo de n
    Passager split(int n);

    int getColor() const { return color; }
    int getGroupAmount() const { return groupAmount; }
};
