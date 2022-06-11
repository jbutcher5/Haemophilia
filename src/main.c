#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
#include <math.h>
#include <stdlib.h>

#include <stdio.h>

#define WIDTH 960
#define HEIGHT 540
#define RL_PROJECTION 0x1701

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
    (Vector3){1.f, 0.0f, 0.0f},
    (Vector2){0.f, 0.f}
};

Vector3 rotateVectorVertical(const Vector3 v, const float theta) {
    Vector3 result = v;

    float horizontalLength = sqrtf(v.x*v.x + v.z*v.z);
    float horizontalNew = cosf(theta) * horizontalLength - sinf(theta) * v.y;

    result.x = cosf(player.theta.x) * horizontalNew;
    result.z = sinf(player.theta.x) * horizontalNew;
    result.y = sinf(theta) * horizontalLength + cosf(theta) * v.y;

    return result;
}

Vector3 rotateVectorHorizontal(const Vector3 v, const float theta) {
    Vector3 result = v;

    result.x = cosf(theta) * v.x - sinf(theta) * v.z;
    result.z = sinf(theta) * v.x + cosf(theta) * v.z;

    return result;
}

Vector3 getPlayerTarget() {
    return rotateVectorHorizontal(
        rotateVectorVertical(player.target, -player.theta.y),
        player.theta.x
    );
}

void UpdatePlayer() {
    Vector2 mouseDelta = GetMouseDelta();
    Vector2 delta = {mouseDelta.x/100, mouseDelta.y/100};

    player.theta.y = MODULO(player.theta.y-delta.y, 2*M_PI);
    player.theta.x = atan2f(player.target.z, player.target.x);

    player.target = rotateVectorHorizontal(
        rotateVectorVertical(player.target, -delta.y),
        delta.x
    );

    Vector3 direction = (Vector3){cosf(player.theta.x)*.25f, 0.f, sinf(player.theta.x)*.25f};

    if (IsKeyDown(KEY_W))
        player.position = Vector3Add(player.position, direction);

    if (IsKeyDown(KEY_D)) {
        Vector3 newDirection = rotateVectorHorizontal(direction, M_PI/2);
        player.position = Vector3Add(player.position, newDirection);
    }

    if (IsKeyDown(KEY_A)) {
        Vector3 newDirection = rotateVectorHorizontal(direction, 3*(M_PI/2));
        player.position = Vector3Add(player.position, newDirection);
    }

    if (IsKeyDown(KEY_S))
        player.position = Vector3Subtract(player.position, direction);

}

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

    while (!WindowShouldClose()) {

        UpdatePlayer();

        BeginDrawing();

        ClearBackground(RAYWHITE);

        StartDisplay();

        DrawCubeV((Vector3){2.f, 0.f, 0.f}, (Vector3){2.f, 2.f, 2.f}, RED);
        DrawCubeV((Vector3){0.f, 0.f, 4.f}, (Vector3){2.f, 3.f, 2.f}, BLUE);

        EndMode3D();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
