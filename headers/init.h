#ifndef INIT_H
#define INIT_H

#include "raylib.h"

// Function prototypes for initialization and de-initialization
void InitializeWindowAndAudio(int screenWidth, int screenHeight);
Music LoadAndPlayMusic(const char *musicPath);
Camera SetupCamera();
Texture2D LoadCubicMapTexture(Image *imMap);
Model LoadMapModel(Image imMap, Texture2D texture);

#endif // INIT_H
