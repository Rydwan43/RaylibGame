#include "game.h"

void UpdateCameraAndCheckCollision(Camera *camera, Color *mapPixels, Texture2D cubicmap, Vector3 mapPosition)
{
    Vector3 oldCamPos = camera->position; // Store old camera position
    UpdateCamera(camera, CAMERA_FIRST_PERSON);

    // Check player collision (we simplify to 2D collision detection)
    Vector2 playerPos = {camera->position.x, camera->position.z};
    float playerRadius = 0.1f; // Collision radius (player is modeled as a cylinder for collision)

    int playerCellX = (int)(playerPos.x - mapPosition.x + 0.5f);
    int playerCellY = (int)(playerPos.y - mapPosition.z + 0.5f);

    // Out-of-limits security check
    if (playerCellX < 0)
        playerCellX = 0;
    else if (playerCellX >= cubicmap.width)
        playerCellX = cubicmap.width - 1;

    if (playerCellY < 0)
        playerCellY = 0;
    else if (playerCellY >= cubicmap.height)
        playerCellY = cubicmap.height - 1;

    // Check map collisions using image data and player position
    for (int y = 0; y < cubicmap.height; y++)
    {
        for (int x = 0; x < cubicmap.width; x++)
        {
            if ((mapPixels[y * cubicmap.width + x].r == 255) && // Collision: white pixel, only check R channel
                (CheckCollisionCircleRec(playerPos, playerRadius,
                                         (Rectangle){mapPosition.x - 0.5f + x * 1.0f, mapPosition.z - 0.5f + y * 1.0f, 1.0f, 1.0f})))
            {
                // Collision detected, reset camera position
                camera->position = oldCamPos;
            }
        }
    }
}

void UpdateMusic(Music music)
{
    UpdateMusicStream(music);
}
