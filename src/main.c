#include <raylib.h>
#include <raymath.h>
#include <math.h>
#include <stdlib.h>

#define WIDTH 960
#define HIEGHT 540

#define MODULO(a, n) fmod((a), (n)) + (((a) < 0) * (n))

typedef struct Player {
    Vector3 position;
    Vector3 size;
    Vector3 target;
    Vector2 theta;
} Player;

static Player player = {
    (Vector3){0.f, 0.0f, 0.0f},
    (Vector3){2.f, 2.f, 2.f},
    (Vector3){4.f, 0.0f, 0.0f},
    (Vector2){0.f, 0.f}
};

Vector3 rotateVectorVertical(const Vector3 v, const float theta) {
    Vector3 result = v;

    result.x = cosf(-theta) * v.x - sinf(-theta) * v.y;
    result.y = sinf(-theta) * v.x + cosf(-theta) * v.y;

    return result;
}

Vector3 rotateVectorHorizontal(const Vector3 v, const float theta) {
    Vector3 result = v;

    result.z = -sinf(-theta) * v.x + cosf(-theta) * v.z;
    result.x = cosf(-theta) * v.x + sinf(-theta) * v.z;

    return result;
}

void UpdatePlayer() {
    Vector2 mouseDelta = GetMouseDelta();

    Vector2 delta = {mouseDelta.x/75, mouseDelta.y/75};

    player.theta.y = MODULO(player.theta.y + delta.y, M_PI);
    player.theta.x = MODULO(player.theta.x + delta.x, 2*M_PI);
    player.target = rotateVectorHorizontal(player.target, delta.x);
    player.target = rotateVectorVertical(player.target, delta.y);

    Vector2 direction = (Vector2){cosf(player.theta.x)*.25f, sinf(player.theta.x)*.25f};

    if (IsKeyDown(KEY_W)) {
        player.position.x += direction.x;
        player.position.z += direction.y;
        player.target.x += direction.x;
        player.target.z += direction.y;
    }
}

void UpdatePlayer(Player* player) {
    Vector2 mouseDelta = GetMouseDelta();

    float deltaY = mouseDelta.y/75;
    float deltaZ = mouseDelta.x/75;

    float totalY = MODULO(player->thetaY + deltaY, M_PI);
    player->thetaZ = MODULO(player->thetaZ + deltaZ, 2*M_PI);
    player->camera.target = rotateVectorZ(player->camera.target, deltaZ);
    player->camera.target = rotateVectorY(player->camera.target, deltaY);
    player->thetaY = totalY;

    if (IsKeyDown(KEY_W)) {
        player->position->x += .25f;
        player->camera.target.x += .25f;
    }

    if (IsKeyDown(KEY_S)) {
        player->position->x -= .25f;
        player->camera.target.x -= .25f;
    }

    if (IsKeyDown(KEY_A)) {
        player->position->z -= .25f;
        player->camera.target.z -= .25f;
    }

    if (IsKeyDown(KEY_D)) {
       player->position->z += .25f;
       player->camera.target.z += .25f;
    }

}

int main(){
    InitWindow(WIDTH, HIEGHT, "Haemophilia");
    DisableCursor();

    SetTargetFPS(60);

    while (!WindowShouldClose()) {

        UpdatePlayer();

        BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode3D(player->camera);
        DrawCubeV((Vector3){2.f, 0.f, 0.f}, (Vector3){2.f, 2.f, 2.f}, RED);
        DrawCubeV((Vector3){0.f, 0.f, 4.f}, (Vector3){2.f, 3.f, 2.f}, BLUE);

        EndMode3D();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
