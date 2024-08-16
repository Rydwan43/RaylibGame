#ifndef RENDER_H
#define RENDER_H

#include "raylib.h"

// Function prototypes for rendering
void RenderScene(Camera camera, Model model, Vector3 mapPosition, Texture2D cubicmap, int playerCellX, int playerCellY);

#endif // RENDER_H
