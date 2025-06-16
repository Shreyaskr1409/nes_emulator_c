#include <raylib.h>
#include <stdio.h>
#include "../bus/bus.h"
#include "../cartridge/cartridge.h"

Bus bus;
cartridge cart;
cpu6502 cpu;
ppu2C02 ppu;

bool bEmulationRun = false;
float fResidualTime = 0.0f;

int main() {

    if (!CartInit(&cart, "./test/nestest.nes")) {
        printf("Failed to find given rom file");
        return 1;
    }
    printf("Cartridge loaded successfully");
    BusInit(&bus, &cpu, &ppu);
    BusInsertCartridge(&bus, &cart);
    BusReset(&bus);

    InitWindow(700, 480, "Nestest");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(BLACK);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}

void HandleInput() {
    float fElapsedTime = GetFrameTime();

    if (IsKeyPressed(KEY_SPACE)) bEmulationRun = !bEmulationRun;
    if (IsKeyPressed(KEY_R)) BusReset(&bus);

    if (bEmulationRun) {
        if (fResidualTime > 0.0f) {
            fResidualTime -= fElapsedTime;
        } else {
            fResidualTime += (1.0f / 60.0f) - fElapsedTime;
            // Implementation of debugging part is left
            // do {
            //     BusClock(&bus);
            // } while (bus.ppu.frame_complete);
        }
    }
}
