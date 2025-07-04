#include "cartridge.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

Mapper pMapper;

bool CartInit(cartridge *cart, const char* sFileName) {
    cart->nMapperId = 0;
    cart->nPRGBanks = 0;
    cart->nCHRBanks = 0;
    cart->bImageValid = false;

    FILE* ifs = fopen(sFileName, "rb");
    if (ifs == NULL) {
        fprintf(stderr, "Error: Could not open file %s\n", sFileName);
        return false;
    }

    fread(&cart->header, sizeof(struct sHeader), 1, ifs);

    if (strncmp(cart->header.name, "NES", 3) != 0) {
        fprintf(stderr, "Error: Invalid iNES header (missing 'NES' signature)\n");
        fclose(ifs);
        return false;
    }

    // Skip the "trainer" if it exists
    if (cart->header.mapper1 & 0x04) {
        fseek(ifs, 512, SEEK_CUR);
    }

    cart->nMapperId = ((cart->header.mapper2 >> 4) << 4) | (cart->header.mapper1 >> 4);
    printf("The mapper ID is: %d\n", cart->nMapperId);

    cart->mirror = (cart->header.mapper1 & 0x01) ? VERTICAL : HORIZONTAL;

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
            return false;
        }
        fread(cart->vPRGMem, sizeof(uint8_t), cart->nPRGBanks*16384, ifs);

        printf("First 256 bytes of PRG ROM:\n");
        for (int i = 0; i < 256; i++) {
            printf("%02X ", cart->vPRGMem[i]);
            if (i == 7) printf(" ");
        }
        printf("\n");

        cart->nCHRBanks = cart->header.chr_rom_chunks;
        if (cart->nCHRBanks * 16384 > sizeof(cart->vCHRMem)) {
            fprintf(stderr, "Size of CHR_ROM too large for the allocated memory. Expected: < %lu, Recieved: %u",
                    sizeof(cart->vCHRMem), (cart->nCHRBanks * 16384));
            fclose(ifs);
            return false;
        }
        fread(cart->vCHRMem, sizeof(uint8_t), cart->nCHRBanks*16384, ifs);

        printf("First 256 bytes of CHR ROM:\n");
        for (int i = 0; i < 256; i++) {
            printf("%02X ", cart->vCHRMem[i]);
            if (i == 7) printf(" ");
        }
        printf("\n");
    }
    if (nFileType == 2) {
        // TODO
    }

    switch (cart->nMapperId) {
        case 0:
            Mapper000Init(&pMapper, cart->nPRGBanks, cart->nCHRBanks);
            cart->pMapper = pMapper;
            break;
        default:
            fprintf(stderr, "Could not find appropriate available mapper.\n");
            return false;
    }

    fclose(ifs);
    return true;
}

bool CartReadFromCpuBus(cartridge *cart, uint16_t addr, uint8_t *data) {
    uint32_t mapped_addr = 0; // temp variable
    // if the mapper is supposed to read from the data provided on the CpuBus,
    // the mapper will map the address such that it can be accessed from the
    // memory without giving any error for index out of bounds
    // else we will pass it on for other devices on CpuBus to handle the data
    if (cart->pMapper.cpuMapRead(&cart->pMapper, addr, &mapped_addr)) {
        *data = cart->vPRGMem[mapped_addr];
        return true;
    }
    return false;
}

bool CartWriteToCpuBus(cartridge *cart, uint16_t addr, uint8_t data) {
    uint32_t mapped_addr = 0;
    if (cart->pMapper.cpuMapWrite(&cart->pMapper, addr, &mapped_addr)) {
        cart->vPRGMem[mapped_addr] = data;
        return true;
    }
    return false;
}

bool CartReadFromPpuBus(cartridge *cart, uint16_t addr, uint8_t *data) {
    uint32_t mapped_addr = 0;
    if (cart->pMapper.ppuMapRead(&cart->pMapper, addr, &mapped_addr)) {
        *data = cart->vCHRMem[mapped_addr];
        return true;
    }
    return false;
}

bool CartWriteToPpuBus(cartridge *cart, uint16_t addr, uint8_t data) {
    uint32_t mapped_addr = 0;
    if (cart->pMapper.ppuMapRead(&cart->pMapper, addr, &mapped_addr)) {
        cart->vCHRMem[mapped_addr] = data;
        return true;
    }
    return false;
}
