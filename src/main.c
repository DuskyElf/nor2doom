#include <stdio.h>
#include <raylib.h>

int main(void) {
    int screen_width = 1920;
    int screen_height = 1080;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screen_width, screen_height, "Nor2Doom");

    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        screen_width = GetScreenWidth();
        screen_height = GetScreenHeight();

        BeginDrawing();
        EndDrawing();
    }

    return 0;
}
