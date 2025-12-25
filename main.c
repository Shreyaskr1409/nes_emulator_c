#include <raylib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "bus/bus.h"
#include "cartridge/cartridge.h"
#include "stc/cstr.h"

Bus bus;
cartridge cart;
cpu6502 cpu;
ppu2C02 ppu;

bool bEmulationRun = false;
float fResidualTime = 0.0f;
hmap_uint16_str mapAsm;
uint8_t nSelectedPalette = 0x00;

void HandleInput();
void DrawCpu(int x, int y);
void DrawRam(int x, int y, uint16_t nAddr, int nRows, int nColumns);
void DrawCode(int x, int y, int nLines, hmap_uint16_str* mapAsm, cpu6502* cpu);
void DrawColorPalette(int x, int y);
void DrawPatternTable(int x, int y);
void DrawNameTable();
void DrawPatternTile(int x, int y, uint8_t id, uint8_t paletteIndex, Texture2D patternTable);
void DrawScreen(ppu2C02* ppu);

int LoadROM();

int main() {
    const int scale = 3;
    InitWindow(256 * scale + 540, 240 * scale, "Nestest");
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

        PpuSetPixelScreen(&ppu, 0, 0, WHITE);

        DrawCpu(debugPanelX, debugStartY);
        DrawCode(debugPanelX, 140, 10, &mapAsm, &cpu);
        DrawColorPalette(debugPanelX, 445);
        DrawPatternTable(debugPanelX, 460);
        // DrawNameTable();
        DrawScreen(&ppu);

        EndDrawing();
    }

    CloseWindow();
    BusDestroy(&bus);

    return 0;
}

int LoadROM() {
    // const char* file_path = "./test/nestest.nes";
    // const char* file_path = "./test/game_roms/smb.nes";
    // const char* file_path = "./test/game_roms/kung_fu.nes";
    // const char* file_path = "./test/game_roms/donkey_kong.nes";
    const char* file_path = "./test/game_roms/ice_climber.nes";

    printf("Attempting to load: %s\n", file_path);
    FILE* test_file = fopen(file_path, "rb");
    if (test_file) {
        printf("File exists and is readable\n");
        fclose(test_file);
    } else {
        printf("ERROR: Cannot open file\n");
        perror("File error");
        return 1;
    }

    if (!CartInit(&cart, file_path)) {
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
    if (IsKeyPressed(KEY_P)) {
        nSelectedPalette = (nSelectedPalette + 1) & 0x07;
    }
    PpuUpdateScreenTexture(&ppu);
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
    const int scale = 3;         // Match your window scale factor
    const int fontSize = 18;     // Slightly larger than 10*scale/2 for better readability
    const int lineHeight = 20;   // Increased line spacing for scaled display

    for (int row = 0; row < nRows; row++) {
        snprintf(buffer, sizeof(buffer), "$%04X:", nAddr);

        for (int col = 0; col < nColumns; col++) {
            uint8_t byte = CpuReadFromBus(&cpu, nAddr);

            char byteStr[4];
            snprintf(byteStr, sizeof(byteStr), " %02X", byte);
            strcat(buffer, byteStr);

            nAddr += 1;
        }

        DrawText(buffer, nRamX, nRamY, fontSize, WHITE);
        nRamY += lineHeight;
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

void DrawColorPalette(int x, int y) {

    const int nSwatchSize = 10;

    for (int p = 0; p < 8; p++) { // For each palette
        for (int s = 0; s < 4; s++) { // For each index
            
            Color col = PpuGetColorFromPaletteRam(&ppu, p, s);

            DrawRectangle(
                x + p * (nSwatchSize * 5) + s * nSwatchSize, 
                y, 
                nSwatchSize, 
                nSwatchSize, 
                col // to be replaced with color
            );
        }
    }

    DrawRectangleLines(
        x + nSelectedPalette * (nSwatchSize * 5) - 1, 
        y - 1, 
        (nSwatchSize * 4) + 2, 
        nSwatchSize + 2,
        WHITE
    );

}

void DrawPatternTable(int x, int y) {
    // Draw second pattern table (left)
    Texture2D patternTable0 = PpuGetPatternTable(&ppu, 0, nSelectedPalette);
    DrawTextureEx(patternTable0, (Vector2){x, y}, 0.0f, 2.0f, WHITE);
    
    // Draw second pattern table (right)
    Texture2D patternTable1 = PpuGetPatternTable(&ppu, 1, nSelectedPalette);
    DrawTextureEx(patternTable1, (Vector2){x+260, y}, 0.0f, 2.0f, WHITE);
}

void DrawPatternTile(int x, int y, uint8_t id, uint8_t paletteIndex, Texture2D patternTable) {
    int tileX = (id & 0x0F) << 3; // Lower 4 bits × 8
    int tileY = ((id >> 4) & 0x0F) << 3; // Upper 4 bits × 8

    Rectangle src = { tileX, tileY, 8, 8 };          // Source region from pattern texture
    Rectangle dest = { x, y, 24, 24 };               // Destination size scaled up 3×
    Vector2 origin = { 0, 0 };                       // No origin offset

    DrawTexturePro(patternTable, src, dest, origin, 0.0f, WHITE);
}

void DrawNameTable() {
    Texture2D patternTable = PpuGetPatternTable(&ppu, 1, nSelectedPalette);
    for (uint8_t y = 0; y < 30; y++) {
        for (uint8_t x = 0; x < 32; x++) {
            // Get tile ID from nametable
            uint8_t id = ppu.tblName[0][y * 32 + x];

            // Compute screen position
            int screenX = x * 24;
            int screenY = y * 24;

            // Draw tile
            DrawPatternTile(screenX, screenY, id, nSelectedPalette, patternTable);
        }
    }
}

void DrawScreen(ppu2C02* ppu) {
    DrawTextureEx(
            PpuGetScreen(ppu),
            (Vector2){0, 0},
            0.0f,      // Rotation
            3.0f,      // Scale
            WHITE      // Tint
            );
}
