#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
#include <stdlib.h>

#include "aabb.h"
#include "more_math.h"
#include "player.h"

#define WIDTH 960
#define HEIGHT 540
#define RL_PROJECTION 0x1701

Player player = {{{-0.125f, -1.f, -0.125f}, {0.125f, 1.f, 0.125f}},
                 {1.f, 0.f, 0.f},
                 {0.f, 0.f},
                 Idle,
                 0};

void StartDisplay(void) {
    rlDrawRenderBatchActive();
    rlMatrixMode(RL_PROJECTION);
    rlPushMatrix();
    rlLoadIdentity();

    float aspect = (float)WIDTH / (float)HEIGHT;

    double top = RL_CULL_DISTANCE_NEAR * tan(80 * .4 * DEG2RAD);
    double right = top * aspect;

    Vector3 camera_position = BoundingBoxCentre(player.hitbox);

    camera_position.y += BoundingBoxSize(player.hitbox).y / 2;

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

int main(void) {
    InitWindow(WIDTH, HEIGHT, "Haemophilia");
    DisableCursor();

    SetTargetFPS(60);

    BoundingBox *objects = malloc(sizeof(BoundingBox) * 3);

    objects[0] = NewAABB((Vector3){2.f, 0.f, 0.f}, (Vector3){2.f, 2.f, 2.f});
    objects[1] = NewAABB((Vector3){0.f, 0.f, 4.f}, (Vector3){2.f, 3.f, 2.f});
    objects[2] =
        NewAABB((Vector3){0.f, -5.f, 0.f}, (Vector3){400.f, 1.f, 400.f});

    Color colours[3] = {GREEN, BROWN, BLUE};

    while (!WindowShouldClose()) {
        UpdatePlayer(&player, objects, 3);

        BeginDrawing();

        ClearBackground(RAYWHITE);

        StartDisplay();

        for (int i = 0; i < 3; i++)
            DrawCubeV(BoundingBoxCentre(objects[i]),
                      BoundingBoxSize(objects[i]), colours[i]);

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            for (int i = 0; i < 3; i++) {
                DrawRay(GetPlayerRay(&player), GREEN);
                if (GetRayCollisionBox(GetPlayerRay(&player), objects[i]).hit) {
                    colours[i] = RED;
                }
            }
        }

        EndMode3D();

        EndDrawing();
    }

    free(objects);

    CloseWindow();

    return 0;
}
