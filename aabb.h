#pragma once
#include <raylib.h>

BoundingBox NewAABB(const Vector3 centre, const Vector3 size);
void UpdatePosition(BoundingBox *box, const Vector3 delta);
void SetPosition(BoundingBox *box, const Vector3 centre);
void SetSize(BoundingBox *box, const Vector3 size);
