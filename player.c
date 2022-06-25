#include <math.h>
#include <raylib.h>
#include <raymath.h>

#include "more_math.h"
#include "player.h"
#include "aabb.h"

#define SIZE player->hitbox.size
#define POSITION player->hitbox.position

void UpdatePlayer(Player *player, AABB *objects, int n) {
    Vector3 player_velocity = Vector3Zero();

    // Check Falling

    AABB foot_collider = player->hitbox;
    foot_collider.position.y -= player->hitbox.size.y / 2;
    foot_collider.size.y = 0.01f;

    bool previous = player->isFalling;

    player->isFalling = !player->isJumping;

    for (int i = 0; i < n && player->isFalling; i++)
        player->isFalling = !IsAABBColliding(foot_collider, objects[i]);

    if (!previous && player->isFalling)
        player->startedFalling = GetTime();

    if (IsKeyPressed(KEY_SPACE)) {
        player->isJumping = true;
        player->startedJumping = GetTime();
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

    Vector3 direction = Vector3Zero();

    float total_rotation = 0.f;
    int keys_down = IsKeyDown(KEY_W) + IsKeyDown(KEY_D) + IsKeyDown(KEY_A) +
                    IsKeyDown(KEY_S);

    if (IsKeyDown(KEY_D))
        total_rotation += TAU / 4;

    if (IsKeyDown(KEY_A))
        total_rotation += TAU * .75f;

    if (IsKeyDown(KEY_S))
        total_rotation += PI;

    if (IsKeyDown(KEY_A) && IsKeyDown(KEY_D)) {
        total_rotation -= TAU;
        keys_down -= 2;
    }

    if (IsKeyDown(KEY_S) && IsKeyDown(KEY_W)) {
        total_rotation -= PI;
        keys_down -= 2;
    }

    if (IsKeyDown(KEY_W) && total_rotation > PI)
        total_rotation += TAU;

    float mean_rotation = total_rotation / keys_down;

    if (keys_down)
        direction = RotateVector3((Vector3){cosf(player->theta.x) * .25f, 0.f,
                                            sinf(player->theta.x) * .25f},
                                  (Vector2){mean_rotation, 0.f});

    direction.x *= 100.f;
    direction.z *= 100.f;

    player_velocity = Vector3Add(player_velocity, direction);

    // Update Position

    if (player->isFalling) {
        player_velocity.y +=
            FallingVelocity(GetTime() - player->startedFalling);
    }

    float dtime;
    if (player->isJumping) {
        player->isJumping = DoJumping((dtime = GetTime() - player->startedJumping));
        player_velocity.y += JumpingVelocity(dtime);
    }

    Vector3 delta_velocity = player_velocity;

    delta_velocity.x *= GetFrameTime();
    delta_velocity.y *= GetFrameTime();
    delta_velocity.z *= GetFrameTime();

    player->hitbox.position =
        Vector3Add(player->hitbox.position, delta_velocity);
}
