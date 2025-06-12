#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef struct Mapper {
    uint8_t nPRGBanks;
    uint8_t nCHRBanks;

    bool (*cpuMapRead)(struct Mapper* mapper, uint16_t addr, uint32_t* mapped_addr);
    bool (*cpuMapWrite)(struct Mapper* mapper, uint16_t addr, uint32_t* mapped_addr);
    bool (*ppuMapRead)(struct Mapper* mapper, uint16_t addr, uint32_t* mapped_addr);
    bool (*ppuMapWrite)(struct Mapper* mapper, uint16_t addr, uint32_t* mapped_addr);
} Mapper;

void MapperInit(Mapper* mapper, uint8_t nPRGBanks, uint8_t nCHRBanks);
