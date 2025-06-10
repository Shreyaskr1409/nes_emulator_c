#include "ppu.h"
#include <stdint.h>

uint8_t PpuReadFromCpuBus(ppu2C02 *ppu, uint16_t addr, bool bReadonly) {
    uint8_t data = 0x00;

    switch (addr)
    {
        case 0x0000: // Control
            break;
        case 0x0001: // Mask
            break;
        case 0x0002: // Status
            break;
        case 0x0003: // OAM Address
            break;
        case 0x0004: // OAM Data
            break;
        case 0x0005: // Scroll
            break;
        case 0x0006: // PPU Address
            break;
        case 0x0007: // PPU Data
            break;
    }

    return data;
}

void PpuWriteToCpuBus(ppu2C02 *ppu, uint16_t addr, uint8_t data) {
    switch (addr)
    {
        case 0x0000: // Control
            break;
        case 0x0001: // Mask
            break;
        case 0x0002: // Status
            break;
        case 0x0003: // OAM Address
            break;
        case 0x0004: // OAM Data
            break;
        case 0x0005: // Scroll
            break;
        case 0x0006: // PPU Address
            break;
        case 0x0007: // PPU Data
            break;
    }
}

uint8_t PpuReadFromPpuBus(ppu2C02 *ppu, uint16_t addr, bool bReadonly) {
    uint8_t data = 0x00;
    addr &= 0x3FFF;

    return data;
}

void PpuWriteToPpuBus(ppu2C02 *ppu, uint16_t addr, uint8_t data) {
    addr &= 0x3FFF;
}

void PpuConnectCartridge(ppu2C02 *ppu, cartridge *cart) {
    ppu->cart = cart;
}
