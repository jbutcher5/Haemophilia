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
    bool isFalling;
} Player;

typedef struct Object {
    Vector3 position;
    Vector3 size;
} Object;

static Player player = {
    (Vector3){0.f, 0.0f, 0.0f},
    (Vector3){.25f, .25f, .25f},
    (Vector3){1.f, 0.0f, 0.0f},
    (Vector2){0.f, 0.f}
};

Vector3 boundingBoxMax(const Vector3 position, const Vector3 size) {
    Vector3 result;
    Vector3 halfSize = (Vector3){size.x*.5f, size.y*.5f, size.z*.5f};

    return Vector3Add(position, halfSize);
}

Vector3 boundingBoxOrigin(const Vector3 position, const Vector3 size) {
    Vector3 halfSize = (Vector3){size.x*.5f, size.y*.5f, size.z*.5f};

    return Vector3Subtract(position, halfSize);
}

bool isPlayerColliding(const Object object) {
    Vector3 playerOrigin = boundingBoxOrigin(player.position, player.size);
    Vector3 objectOrigin = boundingBoxOrigin(object.position, object.size);
    Vector3 playerMax = boundingBoxMax(player.position, player.size);
    Vector3 objectMax = boundingBoxMax(object.position, object.size);

    return (playerOrigin.x <= objectMax.x && playerMax.x >= objectOrigin.x) &&
        (playerOrigin.y <= objectMax.y && playerMax.y >= objectOrigin.y) &&
        (playerOrigin.z <= objectMax.z && playerMax.z >= objectOrigin.z);
}

bool isObjectColliding(const Object a, const Object b) {
    Vector3 aOrigin = boundingBoxOrigin(a.position, a.size);
    Vector3 bOrigin = boundingBoxOrigin(b.position, b.size);
    Vector3 aMax = boundingBoxMax(a.position, a.size);
    Vector3 bMax = boundingBoxMax(b.position, b.size);

    return (aOrigin.x <= bMax.x && aMax.x >= bOrigin.x) &&
        (aOrigin.y <= bMax.y && aMax.y >= bOrigin.y) &&
        (aOrigin.z <= bMax.z && aMax.z >= bOrigin.z);
}

bool isPointColliding(const Vector3 point, const Object object) {
    Vector3 objectOrigin = boundingBoxOrigin(object.position, object.size);
    Vector3 objectMax = boundingBoxMax(object.position, object.size);

    return (point.x >= objectOrigin.x && point.x <= objectMax.x) &&
        (point.y >= objectOrigin.y && point.y <= objectMax.y) &&
        (point.z >= objectOrigin.z && point.z <= objectMax.z);
}

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

    Object redCube = {(Vector3){2.f, 0.f, 0.f}, (Vector3){2.f, 2.f, 2.f}};
    Object blueCube = {(Vector3){0.f, 0.f, 4.f}, (Vector3){2.f, 3.f, 2.f}};
    Object floor = {(Vector3){0.f, -5.f, 0.f}, (Vector3){40.f, 1.f, 40.f}};

    while (!WindowShouldClose()) {

        UpdatePlayer();

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
