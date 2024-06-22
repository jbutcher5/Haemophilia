#pragma once
#include "player.h"
#include <raylib.h>

#define TAU (PI * 2)

Vector3 BoundingBoxMax(const Vector3 position, const Vector3 size);
Vector3 BoundingBoxMin(const Vector3 position, const Vector3 size);
Vector3 BoundingBoxCentre(const BoundingBox box);
Vector3 BoundingBoxSize(const BoundingBox box);
Vector3 RotateVector3(const Vector3 v, const Vector2 theta);
float FallingVelocity(float x);
float JumpingVelocity(float x);
Movement DoJumping(float x);
float RunningVelocity(float x);
