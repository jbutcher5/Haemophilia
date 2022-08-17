#include <math.h>
#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <stdlib.h>

#include "aabb.h"
#include "more_math.h"
#include "player.h"

void UpdatePlayer(Player *player, BoundingBox *objects, int n) {
    Vector3 player_velocity = Vector3Zero();

    // Check Falling

    /// Create foot bounding box

    BoundingBox foot_collider = player->hitbox;
    UpdatePosition(&foot_collider, (Vector3){0.f, -SIZE.y / 2, 0.f});
    SetSize(&foot_collider, (Vector3){SIZE.x, 0.01f, SIZE.z});

    /// Check if the foot bounding box is colliding with anything

    bool previous_falling = player->is_falling;

    player->is_falling = !player->is_jumping;
    int foot_colliding = -1;
    for (int i = 0; i < n && player->is_falling; i++) {
        player->is_falling = !CheckCollisionBoxes(foot_collider, objects[i]);

        if (!player->is_falling) {
            foot_colliding = i;
        }
    }

    if (!previous_falling && player->is_falling)
        player->started_falling = GetTime();

    if (IsKeyPressed(KEY_SPACE)) {
        player->is_jumping = true;
        player->started_jumping = GetTime();
    }

    // Mouse Movements

    Vector2 mouse_delta = GetMouseDelta();
    Vector2 delta = {mouse_delta.x / 100, -mouse_delta.y / 100};

    player->theta.y = atan2f(sqrtf(player->target.x * player->target.x +
                                   player->target.x * player->target.x),
                             player->target.y);
    player->theta.x = atan2f(player->target.z, player->target.x);

    player->target = RotateVector3(player->target, delta);

    // Horizontal Movements

    bool previous_running = player->is_running;
    player->is_running = IsKeyDown(KEY_W);

    if (!previous_running && player->is_running)
        player->started_running = GetTime();

    float velocity_multiplyer = 15.f;

    if (player->is_running)
        velocity_multiplyer =
            RunningVelocity(GetTime() - player->started_running);

    Vector3 direction = {cosf(player->theta.x) * velocity_multiplyer, 0.f,
                         sinf(player->theta.x) * velocity_multiplyer};
    Vector3 cartesian = Vector3Zero();

    if (player->is_running)
        cartesian.x += 1;

    if (IsKeyDown(KEY_S))
        cartesian.x -= 1;

    if (IsKeyDown(KEY_D))
        cartesian.z += 1;

    if (IsKeyDown(KEY_A))
        cartesian.z -= 1;

    if (Vector3Length(cartesian)) {
        float rotation = atan2f(cartesian.z, cartesian.x);
        direction = RotateVector3(direction, (Vector2){rotation, 0.f});
        player_velocity = Vector3Add(player_velocity, direction);
    }

    // Update Position

    if (player->is_falling) {
        player_velocity.y +=
            FallingVelocity(GetTime() - player->started_falling);
    }

    float dtime;
    if (player->is_jumping) {
        player->is_jumping =
            DoJumping((dtime = GetTime() - player->started_jumping));
        player_velocity.y += JumpingVelocity(dtime);
    }

    for (int i = 0; i < n; i++) {
        if (i == foot_colliding && i >= 0) {
            continue;
        } else if (CheckCollisionBoxes(player->hitbox, objects[i])) {
            player_velocity = (Vector3){0.f, player_velocity.y, 0.f};
            break;
        }
    }

    UpdatePosition(&player->hitbox,
                   Vector3Scale(player_velocity, GetFrameTime()));
}

Ray GetPlayerRay(Player *player) {
    return (Ray){BoundingBoxCentre(player->hitbox), player->target};
}
