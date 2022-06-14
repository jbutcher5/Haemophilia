#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>

#include "object.h"
#include "player.h"

#define WIDTH 960
#define HEIGHT 540
#define RL_PROJECTION 0x1701

static Player player = {
    (Vector3){0.f, 0.0f, 0.0f},
    (Vector3){.25f, .25f, .25f},
    (Vector3){1.f, 0.0f, 0.0f},
    (Vector2){0.f, 0.f}
};

void StartDisplay() {
    rlDrawRenderBatchActive();
    rlMatrixMode(RL_PROJECTION);
    rlPushMatrix();
    rlLoadIdentity();

    float aspect = (float)WIDTH/(float)HEIGHT;

    double top = RL_CULL_DISTANCE_NEAR*tan(80*.4*DEG2RAD);
    double right = top*aspect;

    rlFrustum(-right, right, -top, top, RL_CULL_DISTANCE_NEAR, RL_CULL_DISTANCE_FAR);
    rlMatrixMode(RL_MODELVIEW);
    rlLoadIdentity();

    Matrix matView = MatrixLookAt(player.position, Vector3Add(player.position, player.target), (Vector3){0, 1, 0});
    rlMultMatrixf(MatrixToFloat(matView));

    rlEnableDepthTest();
}

int main(){
    InitWindow(WIDTH, HEIGHT, "Haemophilia");
    DisableCursor();

    SetTargetFPS(60);

    Object redCube = {(Vector3){2.f, 0.f, 0.f}, (Vector3){2.f, 2.f, 2.f}};
    Object blueCube = {(Vector3){0.f, 0.f, 4.f}, (Vector3){2.f, 3.f, 2.f}};
    Object floor = {(Vector3){0.f, -5.f, 0.f}, (Vector3){40.f, 1.f, 40.f}};

    while (!WindowShouldClose()) {
        UpdatePlayer(&player);

        BeginDrawing();

        ClearBackground(RAYWHITE);

        StartDisplay();

        DrawCubeV((Vector3){2.f, 0.f, 0.f}, (Vector3){2.f, 2.f, 2.f}, RED);
        DrawCubeV((Vector3){0.f, 0.f, 4.f}, (Vector3){2.f, 3.f, 2.f}, BLUE);
        DrawCubeV(floor.position, floor.size, GREEN);

        EndMode3D();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
