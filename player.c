#include <math.h>
#include <raylib.h>
#include <raymath.h>
#include <stdlib.h>

#include "aabb.h"
#include "more_math.h"
#include "player.h"

void UpdatePlayer(Player *player, BoundingBox *objects, int n) {
    Vector3 player_velocity = Vector3Zero();
    float time = GetTime();

    // Mouse Movements

    Vector2 mouse_delta = GetMouseDelta();
    Vector2 delta = {mouse_delta.x / 100, -mouse_delta.y / 100};

    player->theta.y = atan2f(sqrtf(player->target.x * player->target.x +
                                   player->target.x * player->target.x),
                             player->target.y);
    player->theta.x = atan2f(player->target.z, player->target.x);

    player->target = RotateVector3(player->target, delta);

    // Horizontal Movements

    if (IsKeyPressed(KEY_SPACE) && !player->is_jumping && !player->is_falling) {
        player->is_jumping = true;
        player->started_jumping = time;
    }

    if (IsKeyDown(KEY_W) && !player->is_running) {
        player->is_running = true;
        player->started_running = time;
    }

    float velocity_multiplyer = 15.f;

    if (player->is_running)
        velocity_multiplyer = RunningVelocity(time - player->started_running);

    Vector3 direction = {cosf(player->theta.x) * velocity_multiplyer, 0.f,
                         sinf(player->theta.x) * velocity_multiplyer};
    Vector3 cartesian = {
        IsKeyDown(KEY_W) - IsKeyDown(KEY_S),
        0.f,
        IsKeyDown(KEY_D) - IsKeyDown(KEY_A),
    };

    if (Vector3Length(cartesian)) {
        float rotation = atan2f(cartesian.z, cartesian.x);
        direction = RotateVector3(direction, (Vector2){rotation, 0.f});
        player_velocity = Vector3Add(player_velocity, direction);
    }

    // Update Position

    if (player->is_falling) {
        player_velocity.y += FallingVelocity(time - player->started_falling);
    }

    if (player->is_jumping) {
        float jumping_delta = time - player->started_jumping;

        player->is_jumping = DoJumping(jumping_delta);
        player_velocity.y += JumpingVelocity(jumping_delta);
    }

    BoundingBox foot_collider = player->hitbox;
    UpdatePosition(&foot_collider, (Vector3){0.f, -SIZE.y / 2, 0.f});
    SetSize(&foot_collider, (Vector3){SIZE.x, 0.01f, SIZE.z});

    BoundingBox head_collider = player->hitbox;
    UpdatePosition(&head_collider, (Vector3){0.f, SIZE.y / 2, 0.f});
    SetSize(&head_collider, (Vector3){SIZE.x, 0.01f, SIZE.z});

    for (int i = 0; i < n; i++) {
        if (CheckCollisionBoxes(foot_collider, objects[i])) {
            player->is_falling = false;
        } else if (!CheckCollisionBoxes(foot_collider, objects[i]) &&
                   !player->is_jumping) {
            if (!player->is_falling) {
                player->started_falling = time;
            }

            player->is_falling = true;
        } else if (CheckCollisionBoxes(head_collider, objects[i])) {
            player->is_falling = player->is_falling;
            player->started_falling = time;
            player->is_jumping = false;
        } else if (CheckCollisionBoxes(player->hitbox, objects[i])) {
            player_velocity = (Vector3){0.f, player_velocity.y, 0.f};
        }
    }

    UpdatePosition(&player->hitbox,
                   Vector3Scale(player_velocity, GetFrameTime()));
}

Ray GetPlayerRay(Player *player) {
    return (Ray){BoundingBoxCentre(player->hitbox), player->target};
}
