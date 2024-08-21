#include <render.h>
#include <math.h>
#include <stdio.h>

void RenderGame(Camera camera, Model model, Vector3 mapPosition)
{
    // Initialize variables
    static float hills = 0.0075f;    // Default value in the middle of the range
    static float smoothness = 0.05f; // Default value in the middle of the range
    static int terrainType = 0;      // 0 for hills, 1 for island
    static bool generate = false;    // Button flag

    ClearBackground(RAYWHITE);

    BeginMode3D(camera);
    DrawModel(model, mapPosition, 1.0f, WHITE);
    EndMode3D();

    DrawFPS(10, 10);

    // Create a buffer to hold the camera position string
    char cameraPosText[50];
    snprintf(cameraPosText, sizeof(cameraPosText), "Player pos: X: %.2f Y: %.2f Z: %.2f", camera.position.x, camera.position.y, camera.position.z);

    // Draw the camera position on the screen
    DrawText(cameraPosText, 10, 30, 20, BLACK);

    // Calculate the angle (horizontal angle in degrees)
    float dx = camera.target.x - camera.position.x;
    float dz = camera.target.z - camera.position.z;
    float angle = atan2f(dz, dx) * (180.0f / PI); // Convert to degrees

    // Create a buffer to hold the camera angle string
    char cameraAngleText[50];
    snprintf(cameraAngleText, sizeof(cameraAngleText), "Camera angle: %.2f°", angle);

    // Draw the camera angle on the screen
    DrawText(cameraAngleText, 10, 60, 20, BLACK);
}