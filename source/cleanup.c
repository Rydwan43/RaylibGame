#include "cleanup.h"

void Cleanup(Color *mapPixels, Texture2D cubicmap, Texture2D texture, Model model, Music music)
{
    UnloadImageColors(mapPixels); // Unload color array
    UnloadTexture(cubicmap);      // Unload cubicmap texture
    UnloadTexture(texture);       // Unload map texture
    UnloadModel(model);           // Unload map model

    StopMusicStream(music);
    UnloadMusicStream(music);

    CloseAudioDevice(); // Close audio device
    CloseWindow();      // Close window and OpenGL context
}
