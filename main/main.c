#include "raylib.h"
#include "init.h"
#include "game.h"
#include "render.h"
#include "terrain.h"
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

    Model terrain = GetTerrain(0);
    Model model = LoadMapModel(imMap, cubesTexture);

    Color *mapPixels = LoadImageColors(imMap);
    UnloadImage(imMap);                          // Unload image from RAM
    Vector3 mapPosition = {-16.0f, 0.0f, -8.0f}; // Set model position

    DisableCursor();
    SetTargetFPS(144);

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_G))
        {
            UnloadModel(terrain);

            terrain = GetTerrain(0);
        }

        UpdateCameraAndCheckCollision(&camera);
        UpdateMusic(music);

        int playerCellX = (int)(camera.position.x - mapPosition.x + 0.5f);
        int playerCellY = (int)(camera.position.z - mapPosition.z + 0.5f);

        RenderGame(camera, model, terrain, mapPosition, cubicmap, playerCellX, playerCellY);
    }

    Cleanup(mapPixels, cubicmap, model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture, model, music);
    UnloadModel(terrain); // Clean up terrain model before exiting
    return 0;
}
