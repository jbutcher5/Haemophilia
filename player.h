#pragma once
#include "aabb.h"
#include <raylib.h>

#define SIZE BoundingBoxSize(player->hitbox)
#define POSITION BoundingBoxCentre(player->hitbox)

typedef enum {
    Running,
    Idle,
    Falling,
    Jumping,
} Movement;

typedef struct Player {
    BoundingBox hitbox;
    Vector3 target;
    Vector2 theta;
    Movement action;
    double action_update;
} Player;

void UpdatePlayer(Player *player, BoundingBox *objects, int n);
Ray GetPlayerRay(Player *player);
