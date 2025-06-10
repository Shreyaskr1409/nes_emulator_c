#include "cpu_test.h"
#include "cpu.h"
#include <stdio.h>

uint8_t cpuRam[64 * 1024];
char ines_file[] = "test-roms/nestest.nes";
char log_file[] = "test-roms/nestest.log";

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
                fptr) != 16) {
        perror("Could not read the iNES header (<16 bytes)");
        exit(EXIT_FAILURE);
    }

    // basic header check
    // in the header, the first three bytes are NES and the last one is 0x1A (end of file)
    if (!(header[0] == 'N' && header[1] == 'E' && header[2] == 'S' && header[3] == 0x1A)) {
        perror("Not a valid iNES file");
        exit(EXIT_FAILURE);
    }

    printf("HEADER starts with: %c%c%c \n", header[0], header[1], header[2]);

    uint32_t program_size = header[4] * 16 * 1024;
    if (program_size != 0x4000) {
        printf("WARNING: Expected 16kb program rom. Found %d bytes.\n", program_size);
    }
    printf("PROGRAM SIZE: %d\n", program_size);

    fseek(fptr, 16, SEEK_SET);

    if (fread(&bus->cpuRam[0xC000], 1, program_size, fptr) != program_size) {
        perror("Failed to read the ROM data");
        exit(EXIT_FAILURE);
    }
    print_ram_range(bus, 0xC000, 16);

    fclose(fptr);
    printf("INFO: Success in loading iNES ROM into 0xC000-0xFFFF.\n");
}

void print_ram_range(Bus *bus, uint16_t start_address, int num_bytes) {
    printf("-------------------------------------------------\n");
    printf("cpuRam from 0x%04X:\n", start_address);
    for (int i = 0; i < num_bytes; ++i) {
        // Ensure we don't go out of bounds of the cpuRam array
        if (start_address + i >= CPU_RAM_SIZE) {
            printf(" (End of cpuRam reached)\n");
            break;
        }
        printf("0x%04X: 0x%02X ", start_address + i, bus->cpuRam[start_address + i]);

        if ((i + 1) % 8 == 0) {
            printf("\n");
        }
    }
    printf("-------------------------------------------------\n");
}

int main() {
    Bus bus;
    cpu6502 cpu;

    BusInit(&bus, &cpu);
    printf("-------------------------------------------------\n");
    printf("Bus and CPU initiated.\n");

    loadNestestRom(&bus, ines_file);

    /////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////// TODO /////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////

    // OPENING A FILE TO LOG INTO
    printf("Log file's path is configured as: .%s \n", log_file);
    FILE* log_fptr = fopen(log_file, "a+");
    if (!log_fptr) {
        perror("Error opening a file to log into");
        exit(EXIT_FAILURE);
    }
    printf("Log file is found. Logging has started successfully!\n");
    printf("-------------------------------------------------\n");

    fprintf(log_fptr, "Logging starts \n");
    fflush(log_fptr);

    fclose(log_fptr);
    return 0;
}
