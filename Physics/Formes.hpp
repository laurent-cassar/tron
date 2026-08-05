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

#endif FORMES_H