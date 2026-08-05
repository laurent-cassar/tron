#include "Formes.hpp"
#include <iostream>

bool SphereCollision(Sphere sphere1, Sphere sphere2) { // Returns if there is a collision between sphere1 and sphere2
    return std::sqrt(((sphere1.position.x - sphere2.position.x) * (sphere1.position.x - sphere2.position.x)) +
                     ((sphere1.position.y - sphere2.position.y) * (sphere1.position.y - sphere2.position.y)) +
                     ((sphere1.position.z - sphere2.position.z) * (sphere1.position.z - sphere2.position.z)))
            <= sphere1.rayon + sphere2.rayon;
}; 

bool AABBCollision(AABB aabb1, AABB aabb2) {

};

AABB AABBExpand(AABB aabb, float valueX, float valueY, float valueZ) {
    if (valueX > 0) aabb.maxX += valueX;
    else aabb.minX += valueX;

    if (valueY > 0) aabb.maxY += valueY;
    else aabb.minY += valueY;

    if (valueZ > 0) aabb.maxZ += valueZ;
    else aabb.minZ += valueZ;
    
    return aabb;
}

AABB AABBGrow(AABB aabb, float valueX, float valueY, float valueZ) {
    
    aabb.minX -= valueX;
    aabb.minY -= valueY;
    aabb.minZ -= valueZ;
    
    aabb.maxX += valueX;
    aabb.maxY += valueY;
    aabb.maxZ += valueZ;

    return aabb;
}

AABB AABBMove(AABB aabb, float valueX, float valueY, float valueZ) {

    aabb.minX += valueX;
    aabb.minY += valueY;
    aabb.minZ += valueZ;
    
    aabb.maxX += valueX;
    aabb.maxY += valueY;
    aabb.maxZ += valueZ;

    return aabb;
}

bool AABBIntersectsX(AABB aabb1, AABB aabb2) {
    return aabb1.minX < aabb2.maxX && aabb1.maxX > aabb2.minX;
}

bool AABBIntersectsY(AABB aabb1, AABB aabb2) {
    return aabb1.minX < aabb2.maxY && aabb1.maxY > aabb2.minY;
}

bool AABBIntersectsZ(AABB aabb1, AABB aabb2) {
    return aabb1.minZ < aabb2.maxZ && aabb1.maxZ > aabb2.minZ;
}

bool AABBIntersects(AABB aabb1, AABB aabb2) {
    return AABBIntersectsX(aabb1, aabb2) && AABBIntersectsY(aabb1, aabb2) && AABBIntersectsZ(aabb1, aabb2);
}

float GetClipX(AABB aabb1, AABB aabb2, float deltaX) {
    if (AABBIntersectsY(aabb1, aabb2) && AABBIntersectsZ(aabb1, aabb2)) {
        if (deltaX > 0 && aabb1.maxX <= aabb2.minX) {
            float clip = aabb2.minX - aabb1.maxX;
            if (deltaX > clip) deltaX = clip;
        }

        if (deltaX < 0 && aabb1.minX >= aabb2.maxX) {
            float clip = aabb2.maxX - aabb1.minX;
            if (deltaX < clip) deltaX = clip;
        }
        return deltaX;
    }
    return deltaX;
}

float GetClipY(AABB aabb1, AABB aabb2, float deltaY) {
    if (AABBIntersectsX(aabb1, aabb2) && AABBIntersectsZ(aabb1, aabb2)) {
        if (deltaY > 0 && aabb1.maxY <= aabb2.minY) {
            float clip = aabb2.minY - aabb1.maxY;
            if (deltaY > clip) deltaY = clip;
        }

        if (deltaY < 0 && aabb1.minY >= aabb2.maxY) {
            float clip = aabb2.maxY - aabb1.minY;
            if (deltaY < clip) deltaY = clip;
        }
        return deltaY;
    }
    return deltaY;
}

float GetClipZ(AABB aabb1, AABB aabb2, float deltaZ) {
    if (AABBIntersectsX(aabb1, aabb2) && AABBIntersectsY(aabb1, aabb2)) {
        if (deltaZ > 0 && aabb1.maxZ <= aabb2.minZ) {
            float clip = aabb2.minZ - aabb1.maxZ;
            if (deltaZ > clip) deltaZ = clip;
        }

        if (deltaZ < 0 && aabb1.minZ >= aabb2.maxZ) {
            float clip = aabb2.maxZ - aabb1.minZ;
            if (deltaZ < clip) deltaZ = clip;
        }
        return deltaZ;
    }
    return deltaZ;
}