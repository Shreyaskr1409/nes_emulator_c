#include <raylib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../bus/bus.h"
#include "../cartridge/cartridge.h"
#include "stc/cstr.h"

Bus bus;
cartridge cart;
cpu6502 cpu;
ppu2C02 ppu;

bool bEmulationRun = false;
float fResidualTime = 0.0f;
hmap_uint16_str mapAsm;

void HandleInput();
void DrawCpu(int x, int y);
void DrawRam(int x, int y, uint16_t nAddr, int nRows, int nColumns);
void DrawCode(int x, int y, int nLines, hmap_uint16_str* mapAsm, cpu6502* cpu);
int LoadROM();

int main() {
    const int scale = 3;
    InitWindow(256 * scale + 300, 240 * scale, "Nestest");
    SetTargetFPS(60);

    if (LoadROM() == 1) {
        return 1;
    }

    const int nesScaledWidth = 256 * 3; // 768 pixels
    const int debugPanelX = nesScaledWidth + 20; // 20px margin from NES display
    const int debugStartY = 10; // Start debug info 10px from top
    const int controlsStartY = 600; // Same relative position as before
    
    while (!WindowShouldClose()) {
        HandleInput();

        BeginDrawing();

        ClearBackground(BLACK);

        DrawTextureEx(
                *ppu.texScreen,
                (Vector2){0, 0},
                0.0f,      // Rotation
                3.0f,      // Scale
                WHITE      // Tint
                );

        DrawCpu(debugPanelX, debugStartY);
        DrawCode(debugPanelX, 150, 10, &mapAsm, &cpu);
        // DrawRam(2, 2, 0x0000, 16, 16);
        // DrawRam(2, 182, 0x8000, 16, 16);

        DrawText("SPACE = Run/Stop", debugPanelX, controlsStartY, 15, WHITE);
        DrawText("R = Reset", debugPanelX, controlsStartY + 20, 15, WHITE);
        DrawText("C = Step Instruction", debugPanelX, controlsStartY + 40, 15, WHITE);
        DrawText("F = Step Frame", debugPanelX, controlsStartY + 60, 15, WHITE);

        EndDrawing();
    }

    CloseWindow();
    BusDestroy(&bus);

    return 0;
}

int LoadROM() {
    printf("Attempting to load: ./test/nestest.nes\n");
    FILE* test_file = fopen("./test/nestest.nes", "rb");
    if (test_file) {
        printf("File exists and is readable\n");
        fclose(test_file);
    } else {
        printf("ERROR: Cannot open file ./test/nestest.nes\n");
        perror("File error");
        return 1;
    }
    if (!CartInit(&cart, "./test/nestest.nes")) {
        printf("ERROR: CartInit failed!\n");
        return 1;
    }
    printf("Cartridge loaded successfully\n");
    if (cart.pMapper.cpuMapRead == NULL) {
        printf("ERROR: Mapper cpuMapRead is NULL!\n");
        return 1;
    }
    if (cart.pMapper.cpuMapWrite == NULL) {
        printf("ERROR: Mapper cpuMapWrite is NULL!\n");
        return 1;
    }
    BusInit(&bus, &cpu, &ppu);
    BusInsertCartridge(&bus, &cart);

    printf("Disassembling...\n");
    mapAsm = disassemble(bus.cpu, 0x0000, 0xFFFF);
    BusReset(&bus);

    for (int i = 0; i < 10; i++) {
        printf("Op %02X: %s\n", i, cpu.lookup[i].name);
    }
    
    // for (uint16_t addr = 0xC000; addr < 0xC010; addr++) {
    //     printf("%04X: %02X\n", addr, CpuReadFromBus(&cpu, addr));
    // }
    
    printf("Initialization complete!\n");
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
            do {
                BusClock(&bus);
            } while (!bus.ppu->frame_complete);
            bus.ppu->frame_complete = false;
        }
    } else {
        if (IsKeyPressed(KEY_C)) {
            do {
                BusClock(&bus);
            } while (!CpuComplete(bus.cpu));
            do {
                BusClock(&bus);
            } while (CpuComplete(bus.cpu));
        }
        if (IsKeyPressed(KEY_F)) {
            do {
                BusClock(&bus);
            } while (!bus.ppu->frame_complete);
            do {
                BusClock(&bus);
            } while (!CpuComplete(bus.cpu));
            bus.ppu->frame_complete = false;
        }
    }
    UpdateTexture(*bus.ppu->texScreen, bus.ppu->frameBuffer);
}

void DrawCpu(int x, int y) {
    char buffer[128];
    const int scale = 3;         // Match your window scale factor
    const int fontSize = 15;     // Slightly larger than 10*scale/2 for better readability
    const int lineHeight = 20;   // Increased line spacing for scaled display
    const int flagSpacing = 20;  // Spacing between status flags
    
    // STATUS header and flags
    DrawText("STATUS:", x, y, fontSize, WHITE);
    
    // Draw each flag with consistent spacing
    int flagX = x + 80;
    DrawText("N", flagX, y, fontSize, (bus.cpu->status & N) ? GREEN : RED);
    DrawText("V", flagX + flagSpacing, y, fontSize, (bus.cpu->status & V) ? GREEN : RED);
    DrawText("-", flagX + flagSpacing*2, y, fontSize, (bus.cpu->status & U) ? GREEN : RED);
    DrawText("B", flagX + flagSpacing*3, y, fontSize, (bus.cpu->status & B) ? GREEN : RED);
    DrawText("D", flagX + flagSpacing*4, y, fontSize, (bus.cpu->status & D) ? GREEN : RED);
    DrawText("I", flagX + flagSpacing*5, y, fontSize, (bus.cpu->status & I) ? GREEN : RED);
    DrawText("Z", flagX + flagSpacing*6, y, fontSize, (bus.cpu->status & Z) ? GREEN : RED);
    DrawText("C", flagX + flagSpacing*7, y, fontSize, (bus.cpu->status & C) ? GREEN : RED);
    
    // Registers with proper spacing
    int regY = y + lineHeight;
    snprintf(buffer, sizeof(buffer), "PC: $%04X", bus.cpu->pc);
    DrawText(buffer, x, regY, fontSize, WHITE);
    
    snprintf(buffer, sizeof(buffer), "A: $%02X [%d]", bus.cpu->a, bus.cpu->a);
    DrawText(buffer, x, regY + lineHeight, fontSize, WHITE);
    
    snprintf(buffer, sizeof(buffer), "X: $%02X [%d]", bus.cpu->x, bus.cpu->x);
    DrawText(buffer, x, regY + lineHeight*2, fontSize, WHITE);
    
    snprintf(buffer, sizeof(buffer), "Y: $%02X [%d]", bus.cpu->y, bus.cpu->y);
    DrawText(buffer, x, regY + lineHeight*3, fontSize, WHITE);
    
    snprintf(buffer, sizeof(buffer), "Stack P: $%04X", bus.cpu->stkp);
    DrawText(buffer, x, regY + lineHeight*4, fontSize, WHITE);
}

void DrawRam(int x, int y, uint16_t nAddr, int nRows, int nColumns) {
    int nRamX = x, nRamY = y;
    char buffer[64];
    
    for (int row = 0; row < nRows; row++) {
        snprintf(buffer, sizeof(buffer), "$%04X:", nAddr);
        
        for (int col = 0; col < nColumns; col++) {
            uint8_t byte = CpuReadFromBus(&cpu, nAddr);
            
            char byteStr[4];
            snprintf(byteStr, sizeof(byteStr), " %02X", byte);
            strcat(buffer, byteStr);
            
            nAddr += 1;
        }
        
        DrawText(buffer, nRamX, nRamY, 10, WHITE);
        nRamY += 12;
    }
}

void DrawCode(int x, int y, int nLines, hmap_uint16_str* mapAsm, cpu6502* cpu) {
    const int fontSize = 15;
    const int lineHeight = 20;
    const uint16_t max_addr = 0xFFFF;
    
    // Center PC line
    int centerY = y + (nLines >> 1) * lineHeight;
    
    // Draw PC line (highlighted)
    hmap_uint16_str_iter pc_it = hmap_uint16_str_find(mapAsm, cpu->pc);
    if (pc_it.ref != NULL) {
        DrawText(cstr_str(&pc_it.ref->second), x, centerY, fontSize, GREEN);
    }

    // Draw lines after PC
    int linesDrawn = 1;
    uint16_t next_addr = cpu->pc + 1;
    int nextY = centerY + lineHeight;
    
    while (linesDrawn < nLines && next_addr <= max_addr) {
        hmap_uint16_str_iter it = hmap_uint16_str_find(mapAsm, next_addr);
        if (it.ref != NULL) {
            DrawText(cstr_str(&it.ref->second), x, nextY, fontSize, WHITE);
            nextY += lineHeight;
            linesDrawn++;
        }
        next_addr++;
    }

    // Draw lines before PC
    uint16_t prev_addr = cpu->pc - 1;
    int prevY = centerY - lineHeight;
    linesDrawn = 1; // Reset counter
    
    while (linesDrawn <= (nLines >> 1) && prev_addr <= max_addr) { // prev_addr will wrap around if < 0
        hmap_uint16_str_iter it = hmap_uint16_str_find(mapAsm, prev_addr);
        if (it.ref != NULL) {
            DrawText(cstr_str(&it.ref->second), x, prevY, fontSize, GRAY);
            prevY -= lineHeight;
            linesDrawn++;
        }
        prev_addr--;
    }
}
