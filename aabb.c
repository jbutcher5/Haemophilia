#include <math.h>
#include <raylib.h>
#include <raymath.h>

#include "aabb.h"
#include "more_math.h"

float BoxDistance(Vector3 point, const BoundingBox box) {
    Vector3 half_size = Vector3Scale(BoundingBoxSize(box), .5f);
    point = Vector3Subtract(point, BoundingBoxCentre(box));

    return sqrtf(powf(fmaxf(fabsf(point.x) - half_size.x, 0), 2.f) +
                 powf(fmaxf(fabsf(point.y) - half_size.y, 0), 2.f) +
                 powf(fmaxf(fabsf(point.z) - half_size.z, 0), 2.f));
}

BoundingBox NewAABB(const Vector3 centre, const Vector3 size) {
    return (BoundingBox){BoundingBoxMin(centre, size),
                         BoundingBoxMax(centre, size)};
}

void UpdatePosition(BoundingBox *box, const Vector3 delta) {
    box->min = Vector3Add(box->min, delta);
    box->max = Vector3Add(box->max, delta);
}

void SetPosition(BoundingBox *box, const Vector3 centre) {
    box->min = Vector3Add(box->min, centre);
    box->max = Vector3Add(box->max, centre);
}

void SetSize(BoundingBox *box, const Vector3 size) {
    Vector3 size_delta = Vector3Subtract(size, BoundingBoxSize(*box));

    box->min = Vector3Add(box->min, Vector3Scale(size_delta, -.5));
    box->max = Vector3Add(box->max, Vector3Scale(size_delta, .5));
}
