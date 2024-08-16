#ifndef GAME_H
#define GAME_H

#include "raylib.h"

// Function prototypes for game logic
void UpdateCameraAndCheckCollision(Camera *camera, Color *mapPixels, Texture2D cubicmap, Vector3 mapPosition);
void UpdateMusic(Music music);

#endif // GAME_H
