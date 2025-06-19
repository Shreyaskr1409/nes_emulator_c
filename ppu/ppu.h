#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "../cartridge/cartridge.h"
#include <raylib.h>

typedef struct ppu2C02 {
    uint8_t tblName[2][1024];
    uint8_t tblPalette[32];
    uint8_t tblPattern[2][4096]; // unused
    cartridge* cart;

    Color palScreen[0x40];
    Image* imgScreen;
    Image* imgNameTable[2];
    Image* imgPatternTable[2];

    Texture2D *texScreen;
    Texture2D *texNameTable[2];
    Texture2D *texPatternTable[2];

    bool frame_complete;
} ppu2C02;

void PpuInit(ppu2C02* ppu);
void PpuDestroy(ppu2C02* ppu);

Texture2D PpuGetScreen(ppu2C02* ppu);
Texture2D PpuGetNameTable(ppu2C02* ppu, uint8_t i);
Texture2D PpuGetPatternTable(ppu2C02* ppu, uint8_t i);

void PpuDrawPixelScreen(ppu2C02* ppu, int x, int y, Color color);
void PpuUpdateScreenTexture(ppu2C02* ppu);

uint8_t PpuReadFromCpuBus(ppu2C02* ppu, uint16_t addr, bool bReadonly);
void PpuWriteToCpuBus(ppu2C02* ppu, uint16_t addr, uint8_t data);

uint8_t PpuReadFromPpuBus(ppu2C02* ppu, uint16_t addr, bool bReadonly);
void PpuWriteToPpuBus(ppu2C02* ppu, uint16_t addr, uint8_t data);

void PpuConnectCartridge(ppu2C02* ppu, cartridge *cart);
void PpuClock(ppu2C02*  ppu);
