#include "bus.h"
#include <stdint.h>
#include <string.h>

// simply a clock function
uint32_t nSysClkCounter = 0;

void BusInit(Bus *bus, cpu6502 *cpu, ppu2C02 *ppu) {
    bus->cpu = cpu;
    bus->ppu = ppu;
    CpuInit(bus->cpu);
    PpuInit(bus->ppu);
    CpuConnectBus(bus->cpu, bus);
    memset(bus->cpuRam, 0x00, CPU_RAM_SIZE);
}

void BusDestroy(Bus *bus) {
    PpuDestroy(bus->ppu);
}

void BusWrite(Bus *bus, uint16_t addr, uint8_t data) {

    if (CartWriteToCpuBus(bus->cart, addr, data)) {
        // TODO
    } else if (0x0000<=addr && addr <=0x1FFF) {
        bus->cpuRam[addr & 0x07FF] = data;
    } else if (addr >= 0x2000 && addr <= 0x3FFF) {
        PpuWriteToCpuBus(bus->ppu, addr & 0x0007, data);
    }

    // hopefully we do not have to encounter a situation
    // where the address is out of the ranges
    // if that occurs, I need to add something to handle it
}

uint8_t BusRead(Bus *bus, uint16_t addr, bool bReadOnly) {
    uint8_t data = 0x00;

    if (CartReadFromCpuBus(bus->cart, addr, &data)) {
        // TODO
    }
    if (0x0000<=addr && addr <=0x1FFF) {
        data = bus->cpuRam[addr & 0x07FF];
    } else if (addr >= 0x2000 && addr <= 0x3FFF) {
        data = PpuReadFromCpuBus(bus->ppu, addr & 0x0007, false);
    }

    return data;
}

void BusReset(Bus *bus) {
    CpuReset(bus->cpu);
    bus->nSystemClockCounter = 0;
}

void BusInsertCartridge(Bus *bus, cartridge *cart) {
    bus->cart = cart;
    PpuConnectCartridge(bus->ppu, cart);
}

void BusClock(Bus *bus) {
    PpuClock(bus->ppu);
    if (bus->nSystemClockCounter % 3 == 0) {
        CpuClock(bus->cpu);
    }

    if (bus->ppu->nmi) {
        bus->ppu->nmi = false;
        CpuNMI(bus->cpu);
    }

    nSysClkCounter++;
}
