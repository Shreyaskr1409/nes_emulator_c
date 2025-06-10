#pragma once

#include <stdint.h>

typedef struct cartridge {
    uint8_t vPRGMem[8*1024];
    uint8_t vCHRMem[8*1024];

    uint8_t nMapperId; // do not forget to initialize this to 0
    uint8_t nPRGBanks; // do not forget to initialize this to 0
    uint8_t nCHRBanks; // do not forget to initialize this to 0
} cartridge;
