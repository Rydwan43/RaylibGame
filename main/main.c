#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

#include "init.h"
#include "game.h"
#include "render.h"
#include "terrain.h"
#include "cleanup.h"

static bool showMenu = false;

// Initialize variables
static float hills = 0.0075f;    // Default value in the middle of the range
static float smoothness = 0.05f; // Default value in the middle of the range
static int terrainType = 0;      // 0 for hills, 1 for island
static bool generate = false;    // Button flag

void RenderMenu();

int main(void)
{
    const int screenWidth = 1024;
    const int screenHeight = 768;

    InitializeWindowAndAudio(screenWidth, screenHeight);

    Music music = LoadAndPlayMusic("resources/ambient.ogg");
    Camera camera = SetupCamera();

    Image sandImage = LoadImage("resources/sand.png");
    Image grassImage = LoadImage("resources/grass.png");
    Image mountainImage = LoadImage("resources/mountain.png");

    Image imMap;
    Texture2D cubicmap = LoadCubicMapTexture(&imMap);
    Texture2D cubesTexture = LoadTexture("resources/cubicmap_atlas.png");

    Model terrain = GetTerrain(0, sandImage, grassImage, mountainImage);
    Model model = LoadMapModel(imMap, cubesTexture);

    Color *mapPixels = LoadImageColors(imMap);
    UnloadImage(imMap);                       // Unload image from RAM
    Vector3 mapPosition = {0.0f, 0.0f, 0.0f}; // Set model position

    SetTargetFPS(144);

    while (!WindowShouldClose())
    {
        if (generate)
        {
            generate = false;
            UnloadModel(terrain);
            terrain = GetTerrain(terrainType, sandImage, grassImage, mountainImage);
        }

        UpdateCameraAndCheckCollision(&camera);
        UpdateMusic(music);

        BeginDrawing();
        RenderGame(camera, terrain, mapPosition);

        // Show or hide the menu based on the flag
        if (showMenu)
        {
            RenderMenu();
        }

        int screenWidth = GetScreenWidth();
        int screenHeight = GetScreenHeight();

        // Button to toggle menu visibility
        if (GuiButton((Rectangle){screenWidth - 60, 10, 50, 50}, "Edit"))
        {
            showMenu = !showMenu; // Toggle the menu visibility
        }

        EndDrawing();
    }

    UnloadImage(sandImage);
    UnloadImage(grassImage);
    UnloadImage(mountainImage);
    Cleanup(mapPixels, cubicmap, model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture, model, music);
    UnloadModel(terrain); // Clean up terrain model before exiting
    return 0;
}

void RenderMenu()
{
    // Define the position and size of the menu elements
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    // Positioning
    int sliderWidth = 200;
    int sliderX = screenWidth - sliderWidth - 50;
    int sliderY = 100;
    int sliderSpacing = 60; // Increased spacing for better visual separation

    GuiPanel((Rectangle){sliderX - 40, sliderY - 30, sliderWidth + 80, 280}, "Generate Terrain");

    // Background rectangle for buttons
    Color bgColor = Fade(DARKGRAY, 0.2f); // Slightly transparent background color

    // Hills slider
    DrawText("Hills", sliderX, sliderY, 15, BLACK);
    GuiSliderBar((Rectangle){sliderX, sliderY + 20, sliderWidth, 20}, "0.005", "0.010", &hills, 0.005f, 0.010f);

    // Smoothness slider
    sliderY += sliderSpacing;
    DrawText("Smoothness", sliderX, sliderY, 15, BLACK);
    GuiSliderBar((Rectangle){sliderX, sliderY + 20, sliderWidth, 20}, "0.1", "0.0", &smoothness, 0.0f, 0.1f);

    // Terrain type toggle
    sliderY += sliderSpacing;
    DrawText("Terrain Type", sliderX, sliderY, 15, BLACK);
    if (GuiButton((Rectangle){sliderX, sliderY + 20, sliderWidth, 30}, terrainType == 0 ? "Hills" : "Island"))
    {
        terrainType = !terrainType; // Toggle between 0 and 1
    }

    // Generate button
    sliderY += sliderSpacing;
    if (GuiButton((Rectangle){sliderX, sliderY + 10, sliderWidth, 30}, "Generate"))
    {
        generate = true;
        // Implement the generation logic here if needed
    }
}