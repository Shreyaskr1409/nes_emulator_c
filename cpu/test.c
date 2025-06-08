// #include "cpu.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

uint8_t RAM[64 * 1024];

int main() {
    // LOADING NESTEXT.NES
    // rb here means read in binary mode
    FILE* fptr = fopen("../test-roms/nestest.nes", "rb");
    if (!fptr) {
        perror("could not open ../test-roms/nestest.nes");
        exit(EXIT_FAILURE);
    }
    printf("Opened the nestest.nes file \n");

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
