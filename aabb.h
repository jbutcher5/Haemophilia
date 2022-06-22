#pragma once
#include <raylib.h>

typedef struct AABB {
    Vector3 position;
    Vector3 size;
} AABB;

bool IsAABBColliding(const AABB a, const AABB b);
bool IsPointColliding(const Vector3 point, const AABB box);
