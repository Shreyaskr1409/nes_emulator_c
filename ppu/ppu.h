#pragma once

typedef struct ppu2C02 {} ppu2C02;

uint8_t PpuReadFromCpuBus(uint16_t addr, bool rdonly);
void PpuWriteToCpuBus(uint16_t addr, uint8_t data);