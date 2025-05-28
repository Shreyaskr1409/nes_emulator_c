#pragma once

#include <cstdint>
#include <stdbool.h>

struct cpu6502;

typedef struct Bus {
    struct cpu6502 *cpu;
    uint8_t ram[64 * 1024];
} Bus;

void BusInit(Bus *bus);
void BusDestroy(Bus *bus);

void BusWrite(Bus *bus, uint16_t addr, uint8_t data);
uint8_t BusRead(Bus *bus, uint16_t addr, bool bReadOnly);
