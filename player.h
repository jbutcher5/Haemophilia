#pragma once
#include <raylib.h>
#include "aabb.h"

typedef struct Player {
    AABB hitbox;
    Vector3 target;
    Vector2 theta;
    bool isFalling;
} Player;

void UpdatePlayer(Player* player);
bool isPlayerColliding(Player* player, const AABB object);
