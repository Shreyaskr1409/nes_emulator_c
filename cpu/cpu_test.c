#include "cpu.h"
#include "../bus/bus.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

uint8_t RAM[64 * 1024];
char ines_file[] = "../test-roms/nestest.nes";

void loadNestestRom(Bus *bus, const char* filename) {
    // rb here means read in binary mode
    FILE* fptr = fopen(filename, "rb");
    if (!fptr) {
        perror("could not open nestest.nes");
        exit(EXIT_FAILURE);
    }
    printf("Opened the nestest.nes file \n");

    ///////////////////////////////////////////////
    // information for iNES files headers are taken
    // from https://www.nesdev.org/wiki/INES
    ///////////////////////////////////////////////

    // header of 16bytes
    uint8_t header[16];
    if (fread(header, // to store the data read
                1, // 1 byte read at a time
                16, //16 bytes to read
                fptr)) {
        perror("Could not read the iNES header");
        exit(EXIT_FAILURE);
    }

    // basic header check
    // in the header, the first three bytes are NES and the last one is 0x1A (end of file)
    if (!(header[0] == 'N' && header[1] == 'E' && header[2] == 'S' && header[3] == 0x1A)) {
        perror("Not a valid iNES file");
        exit(EXIT_FAILURE);
    }

    uint8_t program_size = header[4] * 16 * 1024;
    if (program_size != !0x4000) {
        printf("WARNING: Expected 16kb program rom. Found %d bytes.\n", program_size);
    }

    // seek at 16 bytes offset
    fseek(fptr, 16, SEEK_SET);

    if (fread(&bus->ram[0xC000], 1, program_size, fptr) != program_size) {
        perror("Failed to read the ROM data");
        exit(EXIT_FAILURE);
    }

    fclose(fptr);
    printf("INFO: Success in loading iNES ROM into 0xC000-0xFFFF.\n");
}

int main() {
    Bus bus;
    cpu6502 cpu;

    BusInit(&bus);
    CpuInit(&cpu);
    CpuConnectBus(&cpu, &bus);

    loadNestestRom(&bus, ines_file);

    /////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////// TODO /////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////

    // OPENING A FILE TO LOG INTO
    FILE* log_fptr = fopen("../test-roms/nestest.log", "a+");
    if (!log_fptr) {
        perror("Error opening a file to log into");
        exit(EXIT_FAILURE);
    }
    fprintf(log_fptr, "Logging starts \n");
    fflush(log_fptr);


    fclose(log_fptr);
    return 0;
}
