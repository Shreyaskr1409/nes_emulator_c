#include "mapper.h"

void MapperInit(Mapper *mapper, uint8_t nPRGBanks, uint8_t nCHRBanks) {
    mapper->nPRGBanks = nPRGBanks;
    mapper->nCHRBanks = nCHRBanks;
}
