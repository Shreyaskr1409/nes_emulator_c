#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "../cpu/cpu.h"

#define RAM_SIZE (64 * 1024)

typedef struct Bus {
    cpu6502 *cpu;
    uint8_t ram[RAM_SIZE];
} Bus;

void BusInit(Bus *bus);
void BusDestroy(Bus *bus);

void BusWrite(Bus *bus, uint16_t addr, uint8_t data);
uint8_t BusRead(Bus *bus, uint16_t addr, bool bReadOnly);
