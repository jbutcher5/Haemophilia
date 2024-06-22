#include <math.h>
#include <raylib.h>
#include <raymath.h>
#include <stdlib.h>

#include "aabb.h"
#include "more_math.h"
#include "player.h"

void MouseMovements(Player *player) {
    Vector2 mouse_delta = GetMouseDelta();
    Vector2 delta = {mouse_delta.x / 100, -mouse_delta.y / 100};

    player->theta.y = atan2f(sqrtf(player->target.x * player->target.x +
                                   player->target.x * player->target.x),
                             player->target.y);
    player->theta.x = atan2f(player->target.z, player->target.x);

    player->target = RotateVector3(player->target, delta);
}

void HorizontalMovement(Player *player, double time) {
    if (IsKeyPressed(KEY_SPACE) && player->action != Falling &&
        player->action != Jumping) {
        player->action = Jumping;
        player->action_update = time;
    }

    if (IsKeyDown(KEY_W) &&
        (player->action == Idle || player->action == Running)) {
        player->action = Running;
        player->action_update = time;
    }
}

void MovementCalc(Player *player, double time, Vector3 *player_velocity) {
    float velocity_multiplyer = 15.f;

    if (player->action == Running)
        velocity_multiplyer = RunningVelocity(time - player->action_update);

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
        *player_velocity = Vector3Add(*player_velocity, direction);
    }

    // Update Position

    if (player->action == Falling) {
        player_velocity->y += FallingVelocity(time - player->action_update);
    }

    if (player->action == Jumping) {
        float jumping_delta = time - player->action_update;

        player->action = DoJumping(jumping_delta);

        if (player->action == Falling) {
            player->action_update = time;
        } else {
            player_velocity->y += JumpingVelocity(jumping_delta);
        }
    }
}

void CollisionCalc(Player *player, double time, Vector3 *player_velocity,
                   BoundingBox *objects, int n) {
    BoundingBox foot_collider = player->hitbox;
    UpdatePosition(&foot_collider, (Vector3){0.f, -SIZE.y / 2, 0.f});
    SetSize(&foot_collider, (Vector3){SIZE.x, 0.01f, SIZE.z});

    BoundingBox head_collider = player->hitbox;
    UpdatePosition(&head_collider, (Vector3){0.f, SIZE.y / 2, 0.f});
    SetSize(&head_collider, (Vector3){SIZE.x, 0.01f, SIZE.z});

    for (int i = 0; i < n; i++) {
        if (CheckCollisionBoxes(foot_collider, objects[i]) &&
            player->action == Falling) {
            player->action = Idle;
        } else if (CheckCollisionBoxes(head_collider, objects[i])) {
            if (player_velocity->y > 0)
                player_velocity->y = 0;
        } else if (CheckCollisionBoxes(player->hitbox, objects[i])) {
            // TODO: Improve by checking if it is the x, z or both that needs to
            // be zero'ed don't just do both
            *player_velocity = (Vector3){0.f, player_velocity->y, 0.f};
        }

        if (!CheckCollisionBoxes(foot_collider, objects[i]) &&
            player->action != Jumping) {
            if (player->action != Falling) {
                player->action_update = time;
            }

            player->action = Falling;
        }
    }

    UpdatePosition(&player->hitbox,
                   Vector3Scale(*player_velocity, GetFrameTime()));
}

void UpdatePlayer(Player *player, BoundingBox *objects, int n) {
    double time = GetTime();
    Vector3 player_velocity = Vector3Zero();

    MouseMovements(player);
    HorizontalMovement(player, time);
    MovementCalc(player, time, &player_velocity);
    CollisionCalc(player, time, &player_velocity, objects, n);
}

Ray GetPlayerRay(Player *player) {
    return (Ray){BoundingBoxCentre(player->hitbox), player->target};
}
