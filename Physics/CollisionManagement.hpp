#ifndef COLLISION_MANAGEMENT_H
#define COLLISION_MANAGEMENT_H

#include "Formes.hpp"

bool SphereCollision(Sphere sphere1, Sphere sphere2); // Returns if there is a collision between sphere1 and sphere2

bool AABBCollision(AABB aabb1, AABB aabb2);
AABB AABBExpand(AABB aabb, float valueX, float valueY, float valueZ);
AABB AABBGrow(AABB aabb, float valueX, float valueY, float valueZ);
AABB AABBMove(AABB aabb, float valueX, float valueY, float valueZ);

bool AABBIntersectsX(AABB aabb1, AABB aabb2);
bool AABBIntersectsY(AABB aabb1, AABB aabb2);
bool AABBIntersectsZ(AABB aabb1, AABB aabb2);
bool AABBIntersects(AABB aabb1, AABB aabb2);

float GetClipX(AABB aabb1, AABB aabb2, float deltaX);
float GetClipY(AABB aabb1, AABB aabb2, float deltaY);
float GetClipZ(AABB aabb1, AABB aabb2, float deltaZ);


#endif