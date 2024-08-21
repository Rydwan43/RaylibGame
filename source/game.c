#include "game.h"

void UpdateCameraAndCheckCollision(Camera *camera)
{
    Vector3 oldCamPos = camera->position;
    UpdateCamera(camera, CAMERA_FREE);

    Vector2 playerPos = {camera->position.x, camera->position.z};
    float playerRadius = 0.1f;
}

void UpdateMusic(Music music)
{
    UpdateMusicStream(music);
}
