#pragma once
#include <raylib.h>

typedef struct Object {
    Vector3 position;
    Vector3 size;
    Vector2 theta;
} Object;

bool isObjectColliding(const Object a, const Object b);
bool isPointColliding(const Vector3 point, const Object object);
