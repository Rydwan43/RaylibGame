#include "raylib.h"
#include "init.h"
#include "game.h"
#include "render.h"
#include "cleanup.h"

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitializeWindowAndAudio(screenWidth, screenHeight);

    Music music = LoadAndPlayMusic("resources/ambient.ogg");
    Camera camera = SetupCamera();

    Image imMap;
    Texture2D cubicmap = LoadCubicMapTexture(&imMap);
    Texture2D cubesTexture = LoadTexture("resources/cubicmap_atlas.png");

    Model model = LoadMapModel(imMap, cubesTexture);

    Color *mapPixels = LoadImageColors(imMap);
    UnloadImage(imMap);                          // Unload image from RAM
    Vector3 mapPosition = {-16.0f, 0.0f, -8.0f}; // Set model position

    DisableCursor();
    SetTargetFPS(144);

    while (!WindowShouldClose())
    {
        UpdateCameraAndCheckCollision(&camera, mapPixels, cubicmap, mapPosition);
        UpdateMusic(music);

        int playerCellX = (int)(camera.position.x - mapPosition.x + 0.5f);
        int playerCellY = (int)(camera.position.z - mapPosition.z + 0.5f);

        RenderScene(camera, model, mapPosition, cubicmap, playerCellX, playerCellY);
    }

    Cleanup(mapPixels, cubicmap, model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture, model, music);
    return 0;
}
