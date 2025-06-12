#include "mapper.h"
#include <stdint.h>

static bool cpuMapRead(struct Mapper* mapper, uint16_t addr, uint32_t* mapped_addr);
static bool cpuMapWrite(struct Mapper* mapper, uint16_t addr, uint32_t* mapped_addr);
static bool ppuMapRead(struct Mapper* mapper, uint16_t addr, uint32_t* mapped_addr);
static bool ppuMapWrite(struct Mapper* mapper, uint16_t addr, uint32_t* mapped_addr);

void Mapper000Init(Mapper *mapper, uint8_t nPRGBanks, uint8_t nCHRBanks) {
    MapperInit(mapper, nPRGBanks, nCHRBanks);
}

static bool cpuMapRead(struct Mapper* mapper, uint16_t addr, uint32_t* mapped_addr) {
    if (addr >= 0x8000 && addr <= 0xFFFF) {
        // if the no. of banks is only 1 then it is a 16kB ROM
        // else it is 32kB with no mirroring
        *mapped_addr = (uint32_t)addr & (mapper->nPRGBanks > 1 ? 0x7FFF: 0x3FFF);
        return true;
    }
    return false;
}

static bool cpuMapWrite(struct Mapper* mapper, uint16_t addr, uint32_t* mapped_addr) {
    if (addr >= 0x8000 && addr <= 0xFFFF) {
        *mapped_addr = (uint32_t)addr & (mapper->nPRGBanks > 1 ? 0x7FFF: 0x3FFF);
        return true;
    }
    return false;
}

static bool ppuMapRead(struct Mapper* mapper, uint16_t addr, uint32_t* mapped_addr) {
    if (addr >= 0x0000 && addr <= 0x1FFF) {
        *mapped_addr = addr;
        return true;
    }
    return false;
}

static bool ppuMapWrite(struct Mapper* mapper, uint16_t addr, uint32_t* mapped_addr) {
    // Self Explanatory: We would never have a case where we need to write
    // into the character memory
    //
    // if (addr >= 0x0000 && addr <= 0x1FFF) {
    //     return true;
    // }
    return false;
}
