#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
#include <math.h>
#include <stdlib.h>

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

    Matrix matView = MatrixLookAt(player.position, player.target, (Vector3){0.f, 1.f, 0.f});
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
