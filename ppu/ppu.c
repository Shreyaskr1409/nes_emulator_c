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

Texture2D PpuGetPatternTable(ppu2C02* ppu, uint8_t i, uint8_t pattern) {
    if (!ppu->imgPatternTable[i]) {
        ppu->imgPatternTable[i] = (Image*)MemAlloc(sizeof(Image));
        *ppu->imgPatternTable[i] = GenImageColor(128, 128, BLANK); // 16x16 tiles * 8 pixels = 128
    }

    for (uint16_t nTileY = 0; nTileY < 16; nTileY++) {
        for (uint16_t nTileX = 0; nTileX < 16; nTileX++) {
            // each column of tile takes up 16*(8*8)*2 bits of information, or 16*16 bytes
            uint16_t nOffset = (nTileY * 256) + (nTileX * 16);

            for (uint16_t nPixelY = 0; nPixelY < 8; nPixelY++) {

                // X and Y in following explanations are equivalent to nPixelX and nPixelY
                //
                // treat the following like an equivalent of array of 'bits' of size 8
                // where each bit tells lsb/msb of each pixel in the X axis of the array
                // X axis because for each Y, we get a set of 8 bits for values for different X
                uint8_t tile_lsb = PpuReadFromPpuBus(ppu, i*0x1000 + nOffset + (nPixelY+0), true);
                uint8_t tile_msb = PpuReadFromPpuBus(ppu, i*0x1000 + nOffset + (nPixelY+8), true);

                for (uint16_t nPixelX = 0; nPixelX < 8; nPixelX++) {
                    // Choosing color from palette between 0 to 3
                    uint8_t pixel = (tile_lsb & 0x01) + (tile_msb & 0x01);

                    // Shifting bits so that we can get data for the next pixel
                    tile_lsb >>= 1;
                    tile_msb >>= 1;

                    ImageDrawPixel(ppu->imgPatternTable[i],
                            nTileX*8 + (7-nPixelX), // because the LSB (of variable) of vars: tile_lsb and tile_msb
                                                    // represent the rightmost pixel in the row
                            nTileY*8 + nPixelY,
                            GetColorFromPaletteRam(ppu, 1, pixel));
                }
            }
        }
    }

    if (!ppu->texPatternTable[i]) {
        ppu->texPatternTable[i] = (Texture2D*)MemAlloc(sizeof(Texture2D));
        *ppu->texPatternTable[i] = LoadTextureFromImage(*ppu->imgPatternTable[i]);
    } else {
        UpdateTexture(*ppu->texPatternTable[i], ppu->imgPatternTable[i]->data);
    }
    return *ppu->texPatternTable[i];
}

Color GetColorFromPaletteRam(ppu2C02* ppu, uint8_t palette, uint8_t pixel) {
    return ppu->palScreen[PpuReadFromPpuBus(ppu, 0x3F00 + (palette<<2) + pixel, true)];
}

void PpuDrawPixelScreen(ppu2C02* ppu, int x, int y, Color color) {
        DrawTextureEx(
                *ppu->texScreen,
                (Vector2){x, y},
                0.0f,      // Rotation
                3.0f,      // Scale
                color      // Tint
                );
        // ImageDrawPixel(ppu->imgScreen, x, y, color);
}

void PpuUpdateScreenTexture(ppu2C02* ppu) {
        UpdateTexture(*ppu->texScreen, ppu->frameBuffer);
        // UpdateTexture(*ppu->texScreen, ppu->imgScreen->data);
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

    if (CartReadFromPpuBus(ppu->cart, addr, &data)) {
        
    } else if (addr >= 0x0000 && addr <= 0x1FFF) {
        // PATTERN MEMORY

        // The first index is to determine if the data is in 1st 4kB or 2nd 4kB
        // since the 1FFF is the max value, the index will only be 0 or 1
        data = ppu->tblPattern[(addr & 0x1000) >> 12][addr & 0x0FFF];

    } else if (addr >= 0x2000 && addr <= 0x3EFF) {
        // NAMETABLE MEMORY
    } else if (addr >= 0x3F00 && addr <= 0x3FFF) {
        // PALETTE MEMORY
        
        addr &= 0x001F; // Index from palette memory can be obtained by masking it
                        // for the last 5 bits
        if (addr == 0x0010) addr = 0x0000;
        if (addr == 0x0014) addr = 0x0004;
        if (addr == 0x0018) addr = 0x0008;
        if (addr == 0x001C) addr = 0x000C;

        data = ppu->tblPalette[addr];
    }

    return data;
}

void PpuWriteToPpuBus(ppu2C02 *ppu, uint16_t addr, uint8_t data) {
    addr &= 0x3FFF;

    if (CartWriteToPpuBus(ppu->cart, addr, data)) {
        
    } else if (addr >= 0x0000 && addr <= 0x1FFF) {
        // PATTERN MEMORY

        // The first index is to determine if the data is in 1st 4kB or 2nd 4kB
        // since the 1FFF is the max value, the index will only be 0 or 1
        ppu->tblPattern[(addr & 0x1000) >> 12][addr & 0x0FFF] = data;

    } else if (addr >= 0x2000 && addr <= 0x3EFF) {
        // NAMETABLE MEMORY
    } else if (addr >= 0x3F00 && addr <= 0x3FFF) {
        // PALETTE MEMORY
        
        addr &= 0x001F; // Index from palette memory can be obtained by masking it
                        // for the last 5 bits
        if (addr == 0x0010) addr = 0x0000;
        if (addr == 0x0014) addr = 0x0004;
        if (addr == 0x0018) addr = 0x0008;
        if (addr == 0x001C) addr = 0x000C;

        ppu->tblPalette[addr] = data;
    }
}

void PpuConnectCartridge(ppu2C02 *ppu, cartridge *cart) {
    ppu->cart = cart;
}

void PpuClock(ppu2C02* ppu) {
    int x = ppu->cycle - 1;
    int y = ppu->scanline;
    
    if (x >= 0 && x < 256 && y >= 0 && y < 240) {
        // Write to buffer instead of drawing
        int idx = (y * 256 + x) * 4;
        ppu->frameBuffer[idx] = GetRandomValue(0, 1) ? 0xFF : 0x30; // R
        ppu->frameBuffer[idx+1] = ppu->frameBuffer[idx]; // G
        ppu->frameBuffer[idx+2] = ppu->frameBuffer[idx]; // B
        ppu->frameBuffer[idx+3] = 0xFF; // A
    }

    // Timing logic (unchanged)
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
