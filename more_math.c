#include "player.h"
#include <math.h>
#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
#include <stdlib.h>

#define JUMPING_OFFSET .6
#define JUMPING_MULTIPLIER 16
#define FALLING_MULTIPLIER 16
#define RUNNING_OFFSET 20
#define RUNNING_MULTIPLIER 15
#define RUNNING_MAX 30.

Vector3 BoundingBoxMax(const Vector3 position, const Vector3 size) {
    return Vector3Add(position, Vector3Scale(size, .5f));
}

Vector3 BoundingBoxMin(const Vector3 position, const Vector3 size) {
    return Vector3Subtract(position, Vector3Scale(size, .5f));
}

Vector3 BoundingBoxSize(const BoundingBox box) {
    return Vector3Subtract(box.max, box.min);
}

Vector3 BoundingBoxCentre(const BoundingBox box) {
    return Vector3Subtract(box.max, Vector3Scale(BoundingBoxSize(box), .5f));
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

float FallingVelocity(float x) { return -FALLING_MULTIPLIER * x; }

float JumpingVelocity(float x) {
    return -JUMPING_MULTIPLIER * (x - JUMPING_OFFSET);
}

Movement DoJumping(float x) {
    return (x >= 0 && x <= JUMPING_OFFSET) ? Jumping : Falling;
}

float RunningVelocity(float x) {
    return fminf(x * RUNNING_MULTIPLIER + RUNNING_OFFSET, RUNNING_MAX);
}
