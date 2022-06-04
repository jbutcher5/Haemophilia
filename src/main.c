#include <raylib.h>
#include <raymath.h>
#include <math.h>
#include <stdlib.h>

#define WIDTH 960
#define HIEGHT 540

typedef struct Player {
    Vector3* position;
    Vector3 size;
    Camera3D camera;
} Player;

Vector3 rotateVector(const Vector3 v, const float theta[2]) {
    Vector3 result = v;

    // Z matrix rotation
    result.x = cosf(theta[0]) * v.x - sinf(theta[0]) * v.y;
    result.y = sinf(theta[0]) * v.x + cosf(theta[0]) * v.y;

    // Y matrix rotation
    result.z = -sinf(theta[1]) * result.x + cosf(theta[1]) * result.z;
    result.x = cosf(theta[1]) * result.x + sinf(theta[1]) * result.z;

    return result;
}

Player* InitPlayer(const Vector3 position, const Vector3 size) {
    Player* allocation = (Player*)malloc(sizeof(Player));

    allocation->camera = (Camera3D){
        position,
        (Vector3){ 1.0f, 0.0f, 0.0f },
        (Vector3){ 0.0f, 1.0f, 0.0f },
        80.f,
        CAMERA_PERSPECTIVE,
    };

    allocation->position = &allocation->camera.position;
    allocation->size = size;

    return allocation;
}

void UpdatePlayer(Player* player) {
    Vector2 mouseDelta = GetMouseDelta();
    float theta[2] = {(-mouseDelta.y/75), (-mouseDelta.x/75)};

    player->camera.target = rotateVector(player->camera.target, theta);
}

int main(){
    InitWindow(WIDTH, HIEGHT, "Haemophilia");
    DisableCursor();

    Player* player = InitPlayer((Vector3){0.f, 0.f, 0.f}, (Vector3){2.f, 2.f, 2.f});

    SetTargetFPS(60);

    while (!WindowShouldClose()) {

        UpdatePlayer(player);

        BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode3D(player->camera);
        DrawCubeV((Vector3){2.f, 0.f, 0.f}, (Vector3){2.f, 2.f, 2.f}, RED);

        EndMode3D();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
