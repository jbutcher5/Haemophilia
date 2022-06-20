#include <raylib.h>

#include "aabb.h"
#include "more_math.h"

bool isAABBColliding(const AABB a, const AABB b) {
    Vector3 aOrigin = boundingBoxOrigin(a.position, a.size);
    Vector3 bOrigin = boundingBoxOrigin(b.position, b.size);
    Vector3 aMax = boundingBoxMax(a.position, a.size);
    Vector3 bMax = boundingBoxMax(b.position, b.size);

    return (aOrigin.x <= bMax.x && aMax.x >= bOrigin.x) &&
        (aOrigin.y <= bMax.y && aMax.y >= bOrigin.y) &&
        (aOrigin.z <= bMax.z && aMax.z >= bOrigin.z);
}

bool isPointColliding(const Vector3 point, const AABB box) {
    Vector3 objectOrigin = boundingBoxOrigin(box.position, box.size);
    Vector3 objectMax = boundingBoxMax(box.position, box.size);

    return (point.x >= objectOrigin.x && point.x <= objectMax.x) &&
        (point.y >= objectOrigin.y && point.y <= objectMax.y) &&
        (point.z >= objectOrigin.z && point.z <= objectMax.z);
}
