#include "bus.h"
#include <stdint.h>
#include <string.h>

void BusInit(Bus *bus, cpu6502 *cpu) {
    bus->cpu = cpu;
    CpuInit(bus->cpu);
    CpuConnectBus(bus->cpu, bus);
    memset(bus->cpuRam, 0x00, CPU_RAM_SIZE);
}

void BusDestroy(Bus *bus) {}

void BusWrite(Bus *bus, uint16_t addr, uint8_t data) {
    if (0x0000<=addr && addr <=0x1FFF) {
        bus->cpuRam[addr & 0x07FF] = data;
    }

    // hopefully we do not have to encounter a situation
    // where the address is out of the ranges
    // if that occurs, I need to add something to handle it
}

uint8_t BusRead(Bus *bus, uint16_t addr, bool bReadOnly) {
    uint8_t data = 0x00;

    if (0x0000<=addr && addr <=0x1FFF) {
        data = bus->cpuRam[addr & 0x07FF];
    }
    return 0x00;
}
