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

bool RaycastAABB(Ray ray, AABB aabb, float* t) {
    float tmin = 0.0f;
    float tmax = std::numeric_limits<float>::max();

    // X
    float invDirX = 1.0f / ray.direction.x;
    float t1 = (aabb.minX - ray.origin.x) * invDirX;
    float t2 = (aabb.maxX - ray.origin.x) * invDirX;
    float tnearX = std::min(t1, t2);
    float tfarX = std::max(t1, t2);

    tmin = std::max(tmin, tnearX);
    tmax = std::min(tmax, tfarX);

    if (tmin > tfarX || tmax < tnearX) {
        return false;
    }

    // Y
    float invDirY = 1.0f / ray.direction.y;
    t1 = (aabb.minY - ray.origin.y) * invDirY;
    t2 = (aabb.maxY - ray.origin.y) * invDirY;
    float tnearY = std::min(t1, t2);
    float tfarY = std::max(t1, t2);

    tmin = std::max(tmin, tnearY);
    tmax = std::min(tmax, tfarY);

    if (tmin > tfarY || tmax < tnearY) {
        return false;
    }

    // Z
    float invDirZ = 1.0f / ray.direction.z;
    t1 = (aabb.minZ - ray.origin.z) * invDirZ;
    t2 = (aabb.maxZ - ray.origin.z) * invDirZ;
    float tnearZ = std::min(t1, t2);
    float tfarZ = std::max(t1, t2);

    tmin = std::max(tmin, tnearZ);
    tmax = std::min(tmax, tfarZ);

    if (tmin > tfarZ || tmax < tnearZ) {
        return false;
    }

    *t = tmin;
    return true;
}

bool RaycastSphere(Ray ray, Sphere sphere, float* t) {
    Position3 oc = {ray.origin.x - sphere.position.x,
                     ray.origin.y - sphere.position.y,
                     ray.origin.z - sphere.position.z};

    float a = ray.direction.x * ray.direction.x +
              ray.direction.y * ray.direction.y +
              ray.direction.z * ray.direction.z;

    float b = 2.0f * (oc.x * ray.direction.x +
                       oc.y * ray.direction.y +
                       oc.z * ray.direction.z);

    float c = oc.x * oc.x + oc.y * oc.y + oc.z * oc.z - sphere.rayon * sphere.rayon;

    float discriminant = b * b - 4 * a * c;

    if (discriminant < 0) {
        return false; // Pas d'intersection
    } else {
        discriminant = std::sqrt(discriminant);
        float t0 = (-b - discriminant) / (2.0f * a);
        float t1 = (-b + discriminant) / (2.0f * a);

        if (t0 >= 0) {
            *t = t0;
            return true;
        } else if (t1 >= 0) {
            *t = t1;
            return true;
        } else {
            return false;
        }
    }
}

Position3 Normalize(Position3 vec) {
    float length = std::sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
    if (length == 0.0f) return {0.0f, 0.0f, 0.0f};
    return {vec.x / length, vec.y / length, vec.z / length};
}