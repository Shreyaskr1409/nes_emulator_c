#include "cpu.h"
#include "../bus/bus.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void print_ram_range(Bus *bus, uint16_t start_address, int num_bytes);
void loadNestestRom(Bus *bus, const char* filename);
