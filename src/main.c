#include <raylib.h>
#include <stdlib.h>

#define WIDTH 800
#define HEIGHT 450

typedef struct Player {
    Vector3* position;
    Vector3 size;
    Camera3D camera;
} Player;

Player* InitPlayer(Vector3 position, Vector3 size) {
    Player* allocation = (Player*)malloc(sizeof(Player));

    allocation->camera = (Camera3D){
        position,
        (Vector3){ 1.0f, 0.0f, 0.0f },
        (Vector3){ 0.0f, 1.0f, 0.0f },
        45.f,
        CAMERA_PERSPECTIVE,
    };

    allocation->position = &allocation->camera.position;
    allocation->size = size;

    return allocation;
}
/*
void UpdatePlayer(Player* player) {
    UpdateCamera(player->camera);
}

int main(){
    InitWindow(WIDTH, HEIGHT, "Haemophilia");

    Player* player = InitPlayer((Vector3){0.f, 0.f, 0.f}, (Vector3){2.f, 2.f, 2.f});

    SetTargetFPS(60);

    while (!WindowShouldClose()) {

        UpdatePlayer(player);

        BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode3D(*player->camera);
        DrawCubeV((Vector3){2.f, 0.f, 0.f}, (Vector3){2.f, 2.f, 2.f}, RED);

        EndMode3D();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
