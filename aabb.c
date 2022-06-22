#include <raylib.h>

#include "aabb.h"
#include "more_math.h"

bool IsAABBColliding(const AABB a, const AABB b) {
    Vector3 a_origin = BoundingBoxOrigin(a.position, a.size);
    Vector3 b_origin = BoundingBoxOrigin(b.position, b.size);
    Vector3 a_max = BoundingBoxMax(a.position, a.size);
    Vector3 b_max = BoundingBoxMax(b.position, b.size);

    return (a_origin.x <= b_max.x && a_max.x >= b_origin.x) &&
           (a_origin.y <= b_max.y && a_max.y >= b_origin.y) &&
           (a_origin.z <= b_max.z && a_max.z >= b_origin.z);
}

bool IsPointColliding(const Vector3 point, const AABB box) {
    Vector3 object_origin = BoundingBoxOrigin(box.position, box.size);
    Vector3 object_max = BoundingBoxMax(box.position, box.size);

    return (point.x >= object_origin.x && point.x <= object_max.x) &&
           (point.y >= object_origin.y && point.y <= object_max.y) &&
           (point.z >= object_origin.z && point.z <= object_max.z);
}
