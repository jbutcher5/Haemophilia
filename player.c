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

    bool previous_falling = player->is_falling;

    player->is_falling = !player->is_jumping;

    for (int i = 0; i < n && player->is_falling; i++)
        player->is_falling = !IsAABBColliding(foot_collider, objects[i]);

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
        velocity_multiplyer = RunningVelocity(GetTime() - player->started_running);

    Vector3 direction = {cosf(player->theta.x)*velocity_multiplyer, 0.f, sinf(player->theta.x)*velocity_multiplyer};
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
        player->is_jumping = DoJumping((dtime = GetTime() - player->started_jumping));
        player_velocity.y += JumpingVelocity(dtime);
    }

    Vector3 delta_velocity = player_velocity;

    delta_velocity.x *= GetFrameTime();
    delta_velocity.y *= GetFrameTime();
    delta_velocity.z *= GetFrameTime();

    player->hitbox.position =
        Vector3Add(player->hitbox.position, delta_velocity);
}
