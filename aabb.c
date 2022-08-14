#include <math.h>
#include <raylib.h>
#include <raymath.h>

#include "aabb.h"
#include "more_math.h"

bool IsAABBColliding(const AABB a, const AABB b) {
    return (a.min.x <= b.max.x && a.max.x >= b.min.x) &&
           (a.min.y <= b.max.y && a.max.y >= b.min.y) &&
           (a.min.z <= b.max.z && a.max.z >= b.min.z);
}

bool IsPointColliding(const Vector3 point, const AABB box) {
    return (point.x >= box.min.x && point.x <= box.max.x) &&
           (point.y >= box.min.y && point.y <= box.max.y) &&
           (point.z >= box.min.z && point.z <= box.max.z);
}

float BoxDistance(Vector3 point, const AABB box) {
    Vector3 half_size = Vector3Scale(box.size, .5f);
    point = Vector3Subtract(point, box.position);

    return sqrtf(powf(fmaxf(fabsf(point.x) - half_size.x, 0), 2.f) +
                 powf(fmaxf(fabsf(point.y) - half_size.y, 0), 2.f) +
                 powf(fmaxf(fabsf(point.z) - half_size.z, 0), 2.f));
}

AABB NewAABB(const Vector3 centre, const Vector3 size) {
    return (AABB){centre, size, BoundingBoxMax(centre, size),
                  BoundingBoxOrigin(centre, size)};
}

void UpdatePosition(AABB *box, const Vector3 delta) {
    box->position = Vector3Add(box->position, delta);
    box->min = Vector3Add(box->min, delta);
    box->max = Vector3Add(box->max, delta);
}

void SetPosition(AABB *box, const Vector3 centre) {
    box->position = centre;
    box->min = Vector3Add(box->min, centre);
    box->max = Vector3Add(box->max, centre);
}

void SetSize(AABB *box, const Vector3 size) {
    *box = NewAABB(box->position, size);
}
