#pragma once
#include <raylib.h>
#include "aabb.h"

typedef struct Player {
    AABB hitbox;
    Vector3 target;
    Vector2 theta;
    bool is_falling;
    bool is_jumping;
    double started_falling;
    double started_jumping;
} Player;

void UpdatePlayer(Player* player, AABB* objects, int n);
