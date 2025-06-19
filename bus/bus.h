#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "../cpu/cpu.h"
#include "../ppu/ppu.h"
#include "../cartridge/cartridge.h"

#define CPU_RAM_SIZE (2 * 1024)

typedef struct Bus {
    ppu2C02 *ppu;
    cpu6502 *cpu;
    cartridge *cart;
    uint8_t cpuRam[CPU_RAM_SIZE];
    uint32_t nSystemClockCounter;
} Bus;

void BusInit(Bus *bus, cpu6502 *cpu, ppu2C02 *ppu);
void BusDestroy(Bus *bus);

void BusWrite(Bus *bus, uint16_t addr, uint8_t data);
uint8_t BusRead(Bus *bus, uint16_t addr, bool bReadOnly);

void BusInsertCartridge(Bus *bus, cartridge *cart);
void BusReset(Bus *bus);
void BusClock(Bus *bus);
