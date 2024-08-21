#include "init.h"

void InitializeWindowAndAudio(int screenWidth, int screenHeight)
{
    InitWindow(screenWidth, screenHeight, "Program Interaktywny Trójwymiarowy 2024");
    InitAudioDevice();
}

Music LoadAndPlayMusic(const char *musicPath)
{
    Music music = LoadMusicStream(musicPath);
    music.looping = true;
    SetMusicVolume(music, 0.5);
    PlayMusicStream(music);
    return music;
}

Camera SetupCamera()
{
    Camera camera = {0};
    camera.position = (Vector3){0.0f, 100.0f, 100.0f}; // Camera position
    camera.target = (Vector3){50.0f, 25.0f, 50.0f};    // Camera looking at point
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};           // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                               // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;            // Camera projection type
    return camera;
}

Texture2D LoadCubicMapTexture(Image *imMap)
{
    *imMap = LoadImage("resources/cubicmap.png"); // Load cubicmap image (RAM)
    return LoadTextureFromImage(*imMap);          // Convert image to texture to display (VRAM)
}

Model LoadMapModel(Image imMap, Texture2D texture)
{
    Mesh mesh = GenMeshCubicmap(imMap, (Vector3){1.0f, 1.0f, 1.0f});
    Model model = LoadModelFromMesh(mesh);
    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture; // Set map diffuse texture
    return model;
}
