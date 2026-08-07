#pragma once

#include <cstdio>
#include <cmath>
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"

// Petit framework de test "maison" : pas de dépendance externe,
// juste des macros + un compteur pass/fail affiché à la fin.

inline int g_testsPassed = 0;
inline int g_testsFailed = 0;

inline bool ApproxEqual(float a, float b, float epsilon = 0.001f)
{
    return std::fabs(a - b) <= epsilon;
}

inline bool ApproxEqual(const Vector3& a, const Vector3& b, float epsilon = 0.001f)
{
    return ApproxEqual(a.x, b.x, epsilon) && ApproxEqual(a.y, b.y, epsilon) && ApproxEqual(a.z, b.z, epsilon);
}

#define CHECK(condition, message)                                          \
    do {                                                                   \
        if (condition) {                                                   \
            g_testsPassed++;                                               \
            printf("  [OK]   %s\n", message);                             \
        } else {                                                           \
            g_testsFailed++;                                               \
            printf("  [FAIL] %s (ligne %d)\n", message, __LINE__);        \
        }                                                                  \
    } while (0)

#define CHECK_FLOAT_EQ(actual, expected, message)                          \
    CHECK(ApproxEqual((actual), (expected)), message)

#define CHECK_VEC3_EQ(actual, expected, message)                           \
    CHECK(ApproxEqual((actual), (expected)), message)

#define SECTION(name) printf("\n-- %s --\n", name)
