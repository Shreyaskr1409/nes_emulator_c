#include "cartridge.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

void CartInit(cartridge *cart, const char* sFileName) {
    cart->nMapperId = 0;
    cart->nPRGBanks = 0;
    cart->nCHRBanks = 0;
    cart->bImageValid = false;

    FILE* ifs = fopen(sFileName, "rb");
    if (ifs == NULL) {
        fprintf(stderr, "Error: Could not open file %s\n", sFileName);
        return;
    }

    fread(&cart->header, sizeof(struct sHeader), 1, ifs);

    if (strncmp(cart->header.name, "NES", 3) != 0) {
        fprintf(stderr, "Error: Invalid iNES header (missing 'NES' signature)\n");
        fclose(ifs);
        return;
    }

    // Skip the "trainer" if it exists
    if (cart->header.mapper1 & 0x04) {
        fseek(ifs, 512, SEEK_CUR);
    }

    cart->nMapperId = ((cart->header.mapper2 >> 4) << 4) | (cart->header.mapper1 >> 4);

    uint8_t nFileType = 1;
    
    if (nFileType == 0) {
        // TODO
    }
    if (nFileType == 1) {
        cart->nPRGBanks = cart->header.prg_rom_chunks;
        if (cart->nPRGBanks * 16384 > sizeof(cart->vPRGMem)) {
            fprintf(stderr, "Size of PRG_ROM too large for the allocated memory. Expected: < %lu, Recieved: %u",
                    sizeof(cart->vPRGMem), (cart->nPRGBanks * 16384));
            fclose(ifs);
            return;
        }
        fread(cart->vPRGMem, sizeof(uint8_t), cart->nPRGBanks, ifs);


        cart->nCHRBanks = cart->header.chr_rom_chunks;
        if (cart->nCHRBanks * 16384 > sizeof(cart->vCHRMem)) {
            fprintf(stderr, "Size of CHR_ROM too large for the allocated memory. Expected: < %lu, Recieved: %u",
                    sizeof(cart->vCHRMem), (cart->nCHRBanks * 16384));
            fclose(ifs);
            return;
        }
        fread(cart->vCHRMem, sizeof(uint8_t), cart->nCHRBanks, ifs);
    }
    if (nFileType == 2) {
        // TODO
    }

    fclose(ifs);
}
