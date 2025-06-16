#include <stdio.h>
#include <raylib.h>

int main() {

    InitWindow(600, 400, "Nestest");

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(BLUE);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
