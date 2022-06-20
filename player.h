#pragma once
#include <raylib.h>
#include "object.h"

typedef struct Player {
    Vector3 position;
    Vector3 size;
    Vector3 target;
    Vector2 theta;
    bool isFalling;
} Player;

void UpdatePlayer(Player* player);
bool isPlayerColliding(Player* player, const AABB object);
