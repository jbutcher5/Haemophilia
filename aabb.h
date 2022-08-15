#pragma once
#include <raylib.h>

typedef struct AABB {
    Vector3 position;
    Vector3 size;
    Vector3 max;
    Vector3 min;
} AABB;

bool IsAABBColliding(const AABB a, const AABB b);
bool IsPointColliding(const Vector3 point, const AABB box);
float BoxDistance(const Vector3 point, const AABB box);
AABB NewAABB(const Vector3 centre, const Vector3 size);
void UpdatePosition(AABB *box, const Vector3 delta);
void SetPosition(AABB *box, const Vector3 centre);
void SetSize(AABB *box, const Vector3 size);
BoundingBox AsBoundingBox(const AABB *box);
