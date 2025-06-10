#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef struct ppu2C02 {} ppu2C02;

uint8_t PpuReadFromCpuBus(ppu2C02* ppu, uint16_t addr, bool bReadonly);
void PpuWriteToCpuBus(ppu2C02* ppu, uint16_t addr, uint8_t data);

uint8_t PpuReadFromPpuBus(ppu2C02* ppu, uint16_t addr, bool bReadonly);
void PpuWriteToPpuBus(ppu2C02* ppu, uint16_t addr, uint8_t data);
