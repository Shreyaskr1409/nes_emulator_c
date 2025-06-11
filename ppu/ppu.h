#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "../cartridge/cartridge.h"

typedef struct ppu2C02 {
    uint8_t tblName[2][1024];
    uint8_t tblPalette[32];
    uint8_t tblPattern[2][4096]; // unused
    cartridge* cart;
} ppu2C02;

uint8_t PpuReadFromCpuBus(ppu2C02* ppu, uint16_t addr, bool bReadonly);
void PpuWriteToCpuBus(ppu2C02* ppu, uint16_t addr, uint8_t data);

uint8_t PpuReadFromPpuBus(ppu2C02* ppu, uint16_t addr, bool bReadonly);
void PpuWriteToPpuBus(ppu2C02* ppu, uint16_t addr, uint8_t data);

void PpuConnectCartridge(ppu2C02* ppu, cartridge *cart);
void PpuClock(ppu2C02*  ppu);
