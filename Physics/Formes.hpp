#ifndef FORMES_H
#define FORMES_H

struct Position3 {
    float x;
    float y;
    float z;
};

struct Sphere {
    float rayon;
    float diametre;
    float volume;
    float aire;
    Position3 position; // Cela devrait être la position du centre de la sphère
};

struct AABB {
    float minX;
    float maxX;
    float minY;
    float maxY;
    float minZ;
    float maxZ;
};

struct Ray {
    Position3 origin;   // Point de départ du rayon
    Position3 direction; // Direction du rayon (normalisée)
};

#endif FORMES_H