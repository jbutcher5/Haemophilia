#pragma once
#include <raylib.h>

float BoxDistance(Vector3 point, const BoundingBox box);
BoundingBox NewAABB(const Vector3 centre, const Vector3 size);
void UpdatePosition(BoundingBox *box, const Vector3 delta);
void SetPosition(BoundingBox *box, const Vector3 centre);
void SetSize(BoundingBox *box, const Vector3 size);
