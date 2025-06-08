#include "bus.h"
#include <stdint.h>
#include <string.h>

void BusInit(Bus *bus) {
    CpuInit(bus->cpu);
    CpuConnectBus(bus->cpu, bus);
    memset(bus->ram, 0x00, RAM_SIZE);
}

void BusDestroy(Bus *bus) {}

void BusWrite(Bus *bus, uint16_t addr, uint8_t data) {
    if (0x0000<=addr && addr <=0xFFFF) {
        bus->ram[addr] = data;
    }

    // hopefully we do not have to encounter a situation
    // where the address is out of the ranges
    // if that occurs, I need to add something to handle it
}

uint8_t BusRead(Bus *bus, uint16_t addr, bool bReadOnly) {
    if (0x0000<=addr && addr <=0xFFFF) {
        return bus->ram[addr];
    }
    return 0x00;
}
