#include <raylib.h>
#include <raymath.h>
#include <math.h>
#include <stdlib.h>
#include <float.h>

#define WIDTH 960
#define HIEGHT 540

#define MODULO(a, n) fmod((a), (n)) + (((a) < 0) * (n))

typedef struct Player {
    Vector3* position;
    Vector3 size;
    float thetaY;
    float thetaZ;
    Camera3D camera;
} Player;

Vector3 rotateVectorY(const Vector3 v, const float theta) {
    Vector3 result = v;

    result.x = cosf(-theta) * v.x - sinf(-theta) * v.y;
    result.y = sinf(-theta) * v.x + cosf(-theta) * v.y;

    return result;
}

Vector3 rotateVectorZ(const Vector3 v, const float theta) {
    Vector3 result = v;

    result.z = -sinf(-theta) * v.x + cosf(-theta) * v.z;
    result.x = cosf(-theta) * v.x + sinf(-theta) * v.z;

    return result;
}

Player* InitPlayer(const Vector3 position, const Vector3 size) {
    Player* allocation = (Player*)malloc(sizeof(Player));

    allocation->camera = (Camera3D){
        position,
        (Vector3){1073741824.f, 0.0f, 0.0f},
        (Vector3){0.0f, 1.f, 0.0f},
        60.f,
        CAMERA_PERSPECTIVE,
    };

    allocation->position = &allocation->camera.position;
    allocation->size = size;

    allocation->thetaY = 0.f;
    allocation->thetaZ = 0.f;

    return allocation;
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

    Player* player = InitPlayer((Vector3){0.f, 0.f, 0.f}, (Vector3){2.f, 2.f, 2.f});

    SetTargetFPS(60);

    while (!WindowShouldClose()) {

        UpdatePlayer(player);

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
