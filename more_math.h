#ifndef MORE_MATH
#define MORE_MATH

#include <raylib.h>

Vector3 boundingBoxMax(const Vector3 position, const Vector3 size);
Vector3 boundingBoxOrigin(const Vector3 position, const Vector3 size);
Vector3 rotateVector3(const Vector3 v, const Vector2 theta);

#endif
