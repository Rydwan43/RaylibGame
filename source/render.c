#include "render.h"

void RenderGame(Camera camera, Model modeltwo, Model model, Vector3 mapPosition, Texture2D cubicmap, int playerCellX, int playerCellY)
{
    BeginDrawing();
    ClearBackground(RAYWHITE);

    BeginMode3D(camera);
    DrawModel(model, mapPosition, 1.0f, WHITE); // Draw maze map
    DrawModel(modeltwo, mapPosition, 1.0f, WHITE);
    EndMode3D();

    DrawTextureEx(cubicmap, (Vector2){GetScreenWidth() - cubicmap.width * 4.0f - 20, 20.0f}, 0.0f, 4.0f, WHITE);
    DrawRectangleLines(GetScreenWidth() - cubicmap.width * 4 - 20, 20, cubicmap.width * 4, cubicmap.height * 4, GREEN);

    // Draw player position radar
    DrawRectangle(GetScreenWidth() - cubicmap.width * 4 - 20 + playerCellX * 4, 20 + playerCellY * 4, 4, 4, RED);

    DrawFPS(10, 10);

    EndDrawing();
}
