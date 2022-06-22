#include <math.h>
#include <raymath.h>
#include <rlgl.h>
#include <stdlib.h>

#include "more_math.h"

Vector3 BoundingBoxMax(const Vector3 position, const Vector3 size) {
    Vector3 half_size = {size.x * .5f, size.y * .5f, size.z * .5f};
    return Vector3Add(position, half_size);
}

Vector3 BoundingBoxOrigin(const Vector3 position, const Vector3 size) {
    Vector3 half_size = (Vector3){size.x * .5f, size.y * .5f, size.z * .5f};
    return Vector3Subtract(position, half_size);
}

Vector3 RotateVector3(const Vector3 v, const Vector2 theta) {
    Vector3 result = v;

    float horizontal_length = sqrtf(v.x * v.x + v.z * v.z);
    float horizontal_new =
        cosf(theta.y) * horizontal_length - sinf(theta.y) * v.y;
    float theta_x = atan2f(v.z, v.x);

    float x = cosf(theta_x) * horizontal_new;
    float z = sinf(theta_x) * horizontal_new;

    result.x = cosf(theta.x) * x - sinf(theta.x) * z;
    result.z = sinf(theta.x) * x + cosf(theta.x) * z;
    result.y = sinf(theta.y) * horizontal_length + cosf(theta.y) * v.y;

    return result;
}

float FallingVelocity(float x) { return fmaxf(-30 * x, -120); }
