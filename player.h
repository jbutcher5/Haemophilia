#pragma once
#include <raylib.h>
#include "aabb.h"

typedef struct Player {
    AABB hitbox;
    Vector3 target;
    Vector2 theta;
    bool isFalling;
    bool isJumping;
    double startedFalling;
    double startedJumping;
} Player;

void UpdatePlayer(Player* player, AABB* objects, int n);
