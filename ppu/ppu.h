#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "../cartridge/cartridge.h"
#include <raylib.h>

typedef struct ppu2C02 {
    uint8_t tblName[2][1024]; // VRAM
    uint8_t tblPalette[32];

    // 1 half handles sprites and other half handles bg tiles in many cases
    // each half consists of 16*16 tiles where each tile is 8*8 pixels
    // each pixel takes up 2 bits (4 options for colors)
    // the data for these pixels are laid out in the memory in 2 8*8 planes
    // first plane holds the LSB and the other holds the MSB
    // All we need to access these planes is location of the pixel in tile,
    // then we can just lookup the LSB and add offset of 8 to find the MSB
    uint8_t tblPattern[2][4096];
    cartridge* cart;

    Color palScreen[0x40];

    Image* imgScreen;
    Image* imgNameTable[2];
    Image* imgPatternTable[2];

    Texture2D *texScreen;
    Texture2D *texNameTable[2];
    Texture2D *texPatternTable[2];

    // Changed from 256*240*4 to 256*256*4 because last 16 rows are present, but hidden
    uint8_t frameBuffer[256 * 256 * 4]; // 32-bit RGBA

    bool frame_complete;

    int16_t scanline;
    int16_t cycle;
} ppu2C02;

void PpuInit(ppu2C02* ppu);
void PpuDestroy(ppu2C02* ppu);

Texture2D PpuGetScreen(ppu2C02* ppu);
Texture2D PpuGetNameTable(ppu2C02* ppu, uint8_t i);
Texture2D PpuGetPatternTable(ppu2C02* ppu, uint8_t i, uint8_t pattern);

Color GetColorFromPaletteRam(ppu2C02* ppu, uint8_t palette, uint8_t pixel);

void PpuDrawPixelScreen(ppu2C02* ppu, int x, int y, Color color);
void PpuUpdateScreenTexture(ppu2C02* ppu);

uint8_t PpuReadFromCpuBus(ppu2C02* ppu, uint16_t addr, bool bReadonly);
void PpuWriteToCpuBus(ppu2C02* ppu, uint16_t addr, uint8_t data);

uint8_t PpuReadFromPpuBus(ppu2C02* ppu, uint16_t addr, bool bReadonly);
void PpuWriteToPpuBus(ppu2C02* ppu, uint16_t addr, uint8_t data);

void PpuConnectCartridge(ppu2C02* ppu, cartridge *cart);
void PpuClock(ppu2C02*  ppu);
