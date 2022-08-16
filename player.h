#pragma once
#include "aabb.h"
#include <raylib.h>

#define SIZE BoundingBoxSize(player->hitbox)
#define POSITION BoundingBoxCentre(player->hitbox)

typedef struct Player {
    BoundingBox hitbox;
    Vector3 target;
    Vector2 theta;
    bool is_running;
    bool is_falling;
    bool is_jumping;
    double started_running;
    double started_falling;
    double started_jumping;
} Player;

void UpdatePlayer(Player *player, BoundingBox *objects, int n);
Ray GetPlayerRay(Player *player);
