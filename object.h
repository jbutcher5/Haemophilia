#ifndef OBJECT
#define OBJECT

#include <raylib.h>

typedef struct AABB {
    Vector3 position;
    Vector3 size;
} AABB;

bool isAABBColliding(const AABB a, const AABB b);
bool isPointColliding(const Vector3 point, const AABB box);

#endif
