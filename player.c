#include <raylib.h>
#include <math.h>

#include "more_math.h"
#include "player.h"

void UpdatePlayer(Player* player) {
    Vector2 mouseDelta = GetMouseDelta();
    Vector2 delta = {mouseDelta.x/100, -mouseDelta.y/100};

    player->theta.y = atan2f(
        sqrtf(player->target.x*player->target.x + player->target.x*player->target.x),
        player->target.y
    );

    player->theta.x = atan2f(player->target.z, player->target.x);


    player->target = rotateVector3(player->target, delta);

    Vector3 direction = (Vector3){cosf(player->theta.x)*.25f, 0.f, sinf(player->theta.x)*.25f};

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

}
