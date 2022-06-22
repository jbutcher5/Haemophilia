#pragma once
#include <raymath.h>

#define TAU (PI*2)

Vector3 BoundingBoxMax(const Vector3 position, const Vector3 size);
Vector3 BoundingBoxOrigin(const Vector3 position, const Vector3 size);
Vector3 RotateVector3(const Vector3 v, const Vector2 theta);
float FallingVelocity(float x);
