#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "mapper.h"
#include "mapper000.h"


typedef struct cartridge {
    // This shoudl cover most of the mappers
    uint8_t vPRGMem[32*16*1024];
    uint8_t vCHRMem[16*16*1024];

    uint8_t nMapperId; // do not forget to initialize this to 0
    uint8_t nPRGBanks; // do not forget to initialize this to 0
    uint8_t nCHRBanks; // do not forget to initialize this to 0
    
    struct sHeader {
        char name[4];
        uint8_t prg_rom_chunks;
        uint8_t chr_rom_chunks;
        uint8_t mapper1;
        uint8_t mapper2;
        uint8_t prg_ram_size;
        uint8_t tv_system1;
        uint8_t tv_system2;
        char unused[5];
    } header;

    bool bImageValid;

    Mapper pMapper;
} cartridge;

void CartInit(cartridge *cart, const char* sFileName);

bool CartReadFromCpuBus(cartridge* cart, uint16_t addr, uint8_t* data);
bool CartWriteToCpuBus(cartridge* cart, uint16_t addr, uint8_t data);

bool CartReadFromPpuBus(cartridge* cart, uint16_t addr, uint8_t* data);
bool CartWriteToPpuBus(cartridge* cart, uint16_t addr, uint8_t data);
