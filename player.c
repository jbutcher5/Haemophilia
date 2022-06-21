#include <raylib.h>
#include <math.h>
#include <raymath.h>

#include "aabb.h"
#include "more_math.h"
#include "player.h"

#define SIZE player->hitbox.size
#define POSITION player->hitbox.position

void UpdatePlayer(Player* player, AABB* objects, int n) {
    Vector3 player_velocity = Vector3Zero();

    // Check Collisions

    AABB footCollider = player->hitbox;
    footCollider.position.y -= player->hitbox.size.y/2;
    footCollider.size.y = 0.01f;

    bool previous = player->isFalling;

    player->isFalling = true;

    for (int object = 0; object < n; object++) {
        if (isAABBColliding(footCollider, *(objects + object))) {
            player->isFalling = false;
            break;
        }
    }

    if (!previous && player->isFalling)
        player->startedFalling = GetTime();

    // Mouse Movements

    Vector2 mouseDelta = GetMouseDelta();
    Vector2 delta = {mouseDelta.x/100, -mouseDelta.y/100};

    player->theta.y = atan2f(
        sqrtf(player->target.x*player->target.x + player->target.x*player->target.x),
        player->target.y
    );
    player->theta.x = atan2f(player->target.z, player->target.x);

    player->target = rotateVector3(player->target, delta);

    Vector3 direction = (Vector3){cosf(player->theta.x)*.25f, 0.f, sinf(player->theta.x)*.25f};

    // Horizontal Movements

    if (IsKeyDown(KEY_W))
        player->hitbox.position = Vector3Add(player->hitbox.position, direction);

    if (IsKeyDown(KEY_D)) {
        Vector3 newDirection = rotateVector3(direction, (Vector2){PI/2, 0.f});
        player->hitbox.position = Vector3Add(player->hitbox.position, newDirection);
    }

    if (IsKeyDown(KEY_A)) {
        Vector3 newDirection = rotateVector3(direction, (Vector2){3*(PI/2), 0});
        player->hitbox.position = Vector3Add(player->hitbox.position, newDirection);
    }

    if (IsKeyDown(KEY_S))
        player->hitbox.position = Vector3Subtract(player->hitbox.position, direction);

    // Update Position

    if (player->isFalling) {
        player_velocity.y += fallingVelocity(GetTime()-player->startedFalling);
    }

    Vector3 delta_velocity = player_velocity;

    delta_velocity.x *= GetFrameTime();
    delta_velocity.y *= GetFrameTime();
    delta_velocity.z *= GetFrameTime();

    player->hitbox.position = Vector3Add(player->hitbox.position, delta_velocity);

}
