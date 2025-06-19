#include "ppu.h"
#include "raylib.h"
#include <stdint.h>

void PpuInit(ppu2C02 *ppu) {
    ppu->cycle = 0;
    ppu->scanline = 0;

    ppu->palScreen[0x00] = (Color){84, 84, 84, 255};
    ppu->palScreen[0x01] = (Color){0, 30, 116, 255};
    ppu->palScreen[0x02] = (Color){8, 16, 144, 255};
    ppu->palScreen[0x03] = (Color){48, 0, 136, 255};
    ppu->palScreen[0x04] = (Color){68, 0, 100, 255};
    ppu->palScreen[0x05] = (Color){92, 0, 48, 255};
    ppu->palScreen[0x06] = (Color){84, 4, 0, 255};
    ppu->palScreen[0x07] = (Color){60, 24, 0, 255};
    ppu->palScreen[0x08] = (Color){32, 42, 0, 255};
    ppu->palScreen[0x09] = (Color){8, 58, 0, 255};
    ppu->palScreen[0x0A] = (Color){0, 64, 0, 255};
    ppu->palScreen[0x0B] = (Color){0, 60, 0, 255};
    ppu->palScreen[0x0C] = (Color){0, 50, 60, 255};
    ppu->palScreen[0x0D] = (Color){0, 0, 0, 255};
    ppu->palScreen[0x0E] = (Color){0, 0, 0, 255};
    ppu->palScreen[0x0F] = (Color){0, 0, 0, 255};

    ppu->palScreen[0x10] = (Color){152, 150, 152, 255};
    ppu->palScreen[0x11] = (Color){8, 76, 196, 255};
    ppu->palScreen[0x12] = (Color){48, 50, 236, 255};
    ppu->palScreen[0x13] = (Color){92, 30, 228, 255};
    ppu->palScreen[0x14] = (Color){136, 20, 176, 255};
    ppu->palScreen[0x15] = (Color){160, 20, 100, 255};
    ppu->palScreen[0x16] = (Color){152, 34, 32, 255};
    ppu->palScreen[0x17] = (Color){120, 60, 0, 255};
    ppu->palScreen[0x18] = (Color){84, 90, 0, 255};
    ppu->palScreen[0x19] = (Color){40, 114, 0, 255};
    ppu->palScreen[0x1A] = (Color){8, 124, 0, 255};
    ppu->palScreen[0x1B] = (Color){0, 118, 40, 255};
    ppu->palScreen[0x1C] = (Color){0, 102, 120, 255};
    ppu->palScreen[0x1D] = (Color){0, 0, 0, 255};
    ppu->palScreen[0x1E] = (Color){0, 0, 0, 255};
    ppu->palScreen[0x1F] = (Color){0, 0, 0, 255};

    ppu->palScreen[0x20] = (Color){236, 238, 236, 255};
    ppu->palScreen[0x21] = (Color){76, 154, 236, 255};
    ppu->palScreen[0x22] = (Color){120, 124, 236, 255};
    ppu->palScreen[0x23] = (Color){176, 98, 236, 255};
    ppu->palScreen[0x24] = (Color){228, 84, 236, 255};
    ppu->palScreen[0x25] = (Color){236, 88, 180, 255};
    ppu->palScreen[0x26] = (Color){236, 106, 100, 255};
    ppu->palScreen[0x27] = (Color){212, 136, 32, 255};
    ppu->palScreen[0x28] = (Color){160, 170, 0, 255};
    ppu->palScreen[0x29] = (Color){116, 196, 0, 255};
    ppu->palScreen[0x2A] = (Color){76, 208, 32, 255};
    ppu->palScreen[0x2B] = (Color){56, 204, 108, 255};
    ppu->palScreen[0x2C] = (Color){56, 180, 204, 255};
    ppu->palScreen[0x2D] = (Color){60, 60, 60, 255};
    ppu->palScreen[0x2E] = (Color){0, 0, 0, 255};
    ppu->palScreen[0x2F] = (Color){0, 0, 0, 255};

    ppu->palScreen[0x30] = (Color){236, 238, 236, 255};
    ppu->palScreen[0x31] = (Color){168, 204, 236, 255};
    ppu->palScreen[0x32] = (Color){188, 188, 236, 255};
    ppu->palScreen[0x33] = (Color){212, 178, 236, 255};
    ppu->palScreen[0x34] = (Color){236, 174, 236, 255};
    ppu->palScreen[0x35] = (Color){236, 174, 212, 255};
    ppu->palScreen[0x36] = (Color){236, 180, 176, 255};
    ppu->palScreen[0x37] = (Color){228, 196, 144, 255};
    ppu->palScreen[0x38] = (Color){204, 210, 120, 255};
    ppu->palScreen[0x39] = (Color){180, 222, 120, 255};
    ppu->palScreen[0x3A] = (Color){168, 226, 144, 255};
    ppu->palScreen[0x3B] = (Color){152, 226, 180, 255};
    ppu->palScreen[0x3C] = (Color){160, 214, 228, 255};
    ppu->palScreen[0x3D] = (Color){160, 162, 160, 255};
    ppu->palScreen[0x3E] = (Color){0, 0, 0, 255};
    ppu->palScreen[0x3F] = (Color){0, 0, 0, 255};


    ppu->imgScreen = (Image*)MemAlloc(sizeof(Image));
    *ppu->imgScreen = GenImageColor(256, 240, BLANK); // Use BLANK for transparency
    ppu->texScreen = (Texture2D*)MemAlloc(sizeof(Texture2D));
    *ppu->texScreen = LoadTextureFromImage(*ppu->imgScreen);

    ppu->imgNameTable[0] = (Image*)MemAlloc(sizeof(Image));
    *ppu->imgNameTable[0] = GenImageColor(256, 240, BLANK);
    ppu->texNameTable[0] = (Texture2D*)MemAlloc(sizeof(Texture2D));
    *ppu->texNameTable[0] = LoadTextureFromImage(*ppu->imgNameTable[0]);

    ppu->imgNameTable[1] = (Image*)MemAlloc(sizeof(Image));
    *ppu->imgNameTable[1] = GenImageColor(256, 240, BLANK);
    ppu->texNameTable[1] = (Texture2D*)MemAlloc(sizeof(Texture2D));
    *ppu->texNameTable[1] = LoadTextureFromImage(*ppu->imgNameTable[1]);

    ppu->imgPatternTable[0] = (Image*)MemAlloc(sizeof(Image));
    *ppu->imgPatternTable[0] = GenImageColor(128, 128, BLANK);
    ppu->texPatternTable[0] = (Texture2D*)MemAlloc(sizeof(Texture2D));
    *ppu->texPatternTable[0] = LoadTextureFromImage(*ppu->imgPatternTable[0]);

    ppu->imgPatternTable[1] = (Image*)MemAlloc(sizeof(Image));
    *ppu->imgPatternTable[1] = GenImageColor(128, 128, BLANK);
    ppu->texPatternTable[1] = (Texture2D*)MemAlloc(sizeof(Texture2D));
    *ppu->texPatternTable[1] = LoadTextureFromImage(*ppu->imgPatternTable[1]);

    ppu->frame_complete = false;
}

Texture2D PpuGetScreen(ppu2C02* ppu) {
    return *ppu->texScreen;
}

Texture2D PpuGetNameTable(ppu2C02* ppu, uint8_t i) {
    return *ppu->texNameTable[i];
}

Texture2D PpuGetPatternTable(ppu2C02* ppu, uint8_t i) {
    return *ppu->texPatternTable[i];
}

void PpuDrawPixelScreen(ppu2C02* ppu, int x, int y, Color color) {
        ImageDrawPixel(ppu->imgScreen, x, y, color);
}

void PpuUpdateScreenTexture(ppu2C02* ppu) {
        UpdateTexture(*ppu->texScreen, ppu->imgScreen->data);
}

void PpuDestroy(ppu2C02* ppu)
{
    // Unload Textures from GPU memory
    UnloadTexture(*ppu->texScreen);
    UnloadTexture(*ppu->texNameTable[0]);
    UnloadTexture(*ppu->texNameTable[1]);
    UnloadTexture(*ppu->texPatternTable[0]);
    UnloadTexture(*ppu->texPatternTable[1]);

    // Unload Images from CPU memory
    UnloadImage(*ppu->imgScreen);
    UnloadImage(*ppu->imgNameTable[0]);
    UnloadImage(*ppu->imgNameTable[1]);
    UnloadImage(*ppu->imgPatternTable[0]);
    UnloadImage(*ppu->imgPatternTable[1]);

    // Free the memory allocated for the Image and Texture2D pointers themselves
    MemFree(ppu->imgScreen);
    MemFree(ppu->texScreen);
    MemFree(ppu->imgNameTable[0]);
    MemFree(ppu->texNameTable[0]);
    MemFree(ppu->imgNameTable[1]);
    MemFree(ppu->texNameTable[1]);
    MemFree(ppu->imgPatternTable[0]);
    MemFree(ppu->texPatternTable[0]);
    MemFree(ppu->imgPatternTable[1]);
    MemFree(ppu->texPatternTable[1]);

    // Finally, free the ppu2C02 struct itself
    MemFree(ppu);
}

uint8_t PpuReadFromCpuBus(ppu2C02 *ppu, uint16_t addr, bool bReadonly) {
    uint8_t data = 0x00;

    switch (addr)
    {
        case 0x0000: // Control
            break;
        case 0x0001: // Mask
            break;
        case 0x0002: // Status
            break;
        case 0x0003: // OAM Address
            break;
        case 0x0004: // OAM Data
            break;
        case 0x0005: // Scroll
            break;
        case 0x0006: // PPU Address
            break;
        case 0x0007: // PPU Data
            break;
    }

    return data;
}

void PpuWriteToCpuBus(ppu2C02 *ppu, uint16_t addr, uint8_t data) {
    switch (addr)
    {
        case 0x0000: // Control
            break;
        case 0x0001: // Mask
            break;
        case 0x0002: // Status
            break;
        case 0x0003: // OAM Address
            break;
        case 0x0004: // OAM Data
            break;
        case 0x0005: // Scroll
            break;
        case 0x0006: // PPU Address
            break;
        case 0x0007: // PPU Data
            break;
    }
}

uint8_t PpuReadFromPpuBus(ppu2C02 *ppu, uint16_t addr, bool bReadonly) {
    uint8_t data = 0x00;
    addr &= 0x3FFF;

    return data;
}

void PpuWriteToPpuBus(ppu2C02 *ppu, uint16_t addr, uint8_t data) {
    addr &= 0x3FFF;
}

void PpuConnectCartridge(ppu2C02 *ppu, cartridge *cart) {
    ppu->cart = cart;
}

void PpuClock(ppu2C02* ppu) {
    // Fake some noise for the screen
    int x = ppu->cycle - 1;
    int y = ppu->scanline;
    
    // Only draw to screen if within visible area
    if (x >= 0 && x < 256 && y >= 0 && y < 240) {
        // Choose between two colors (0x3F or 0x30) randomly
        Color pixel_color = (GetRandomValue(0, 1)) ? 
            GetColor(0x3F3F3FFF) :  // Dark gray
            GetColor(0x303030FF);   // Slightly darker gray
        
        // Draw pixel directly to screen or to a render texture
        // Note: In a real emulator, you'd want to buffer this to a texture
        // and render once per frame for performance
        DrawPixel(x, y, pixel_color);
    }

    // Advance renderer timing
    ppu->cycle++;
    if (ppu->cycle >= 341) {
        ppu->cycle = 0;
        ppu->scanline++;
        if (ppu->scanline >= 261) {
            ppu->scanline = -1;
            ppu->frame_complete = true;
        }
    }
}
