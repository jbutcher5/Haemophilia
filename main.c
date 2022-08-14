#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
#include <stdlib.h>

#include "aabb.h"
#include "player.h"

#define WIDTH 960
#define HEIGHT 540
#define RL_PROJECTION 0x1701

Player player = {
    {{0.f, 0.f, 0.f}, {.25f, 2.f, .25f}, {0.125f, 1.f, 0.125f}, {-0.125f, -1.f, -0.125f}}, {1.f, 0.f, 0.f}, {0.f, 0.f}, false, true, false, 0, 0, 0};

void StartDisplay() {
    rlDrawRenderBatchActive();
    rlMatrixMode(RL_PROJECTION);
    rlPushMatrix();
    rlLoadIdentity();

    float aspect = (float)WIDTH / (float)HEIGHT;

    double top = RL_CULL_DISTANCE_NEAR * tan(80 * .4 * DEG2RAD);
    double right = top * aspect;

    Vector3 camera_position = player.hitbox.position;
    camera_position.y += player.hitbox.size.y / 2;

    rlFrustum(-right, right, -top, top, RL_CULL_DISTANCE_NEAR,
              RL_CULL_DISTANCE_FAR);
    rlMatrixMode(RL_MODELVIEW);
    rlLoadIdentity();

    Matrix mat_view = MatrixLookAt(camera_position,
                                   Vector3Add(camera_position, player.target),
                                   (Vector3){0, 1, 0});
    rlMultMatrixf(MatrixToFloat(mat_view));

    rlEnableDepthTest();
}

int main() {
    InitWindow(WIDTH, HEIGHT, "Haemophilia");
    DisableCursor();

    SetTargetFPS(60);

    AABB *objects = malloc(sizeof(AABB) * 3);

    objects[0] = NewAABB((Vector3){2.f, 0.f, 0.f}, (Vector3){2.f, 2.f, 2.f});
    objects[1] = NewAABB((Vector3){0.f, 0.f, 4.f}, (Vector3){2.f, 3.f, 2.f});
    objects[2] = NewAABB((Vector3){0.f, -5.f, 0.f}, (Vector3){400.f, 1.f, 400.f});

    while (!WindowShouldClose()) {
        UpdatePlayer(&player, objects, 3);

        BeginDrawing();

        ClearBackground(RAYWHITE);

        StartDisplay();

        DrawCubeV(objects[0].position, objects[0].size, RED);
        DrawCubeV(objects[1].position, objects[1].size, BLUE);
        DrawCubeV(objects[2].position, objects[2].size, GREEN);

        EndMode3D();

        EndDrawing();
    }

    free(objects);

    CloseWindow();

    return 0;
}
