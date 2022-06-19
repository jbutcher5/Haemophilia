#include "more_math.h"
#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
#include <math.h>
#include <stdlib.h>

Vector3 boundingBoxMax(const Vector3 position, const Vector3 size) {
    Vector3 halfSize = {size.x*.5f, size.y*.5f, size.z*.5f};
    return Vector3Add(position, halfSize);
}

Vector3 boundingBoxOrigin(const Vector3 position, const Vector3 size) {
    Vector3 halfSize = (Vector3){size.x*.5f, size.y*.5f, size.z*.5f};
    return Vector3Subtract(position, halfSize);
}

Vector3 rotateVector3(const Vector3 v, const Vector2 theta) {
    Vector3 result = v;

    float horizontalLength = sqrtf(v.x*v.x + v.z*v.z);
    float horizontalNew = cosf(theta.y) * horizontalLength - sinf(theta.y) * v.y;
    float thetaX = atan2f(v.z, v.x);

    float x = cosf(thetaX) * horizontalNew;
    float z = sinf(thetaX) * horizontalNew;

    result.x = cosf(theta.x) * x - sinf(theta.x) * z;
    result.z = sinf(theta.x) * x + cosf(theta.x) * z;
    result.y = sinf(theta.y) * horizontalLength + cosf(theta.y) * v.y;

    return result;
}
