#pragma once
#include "cpu.h"
#include <stdint.h>

uint8_t IMP(cpu6502 *cpu);
uint8_t IMM(cpu6502 *cpu);
uint8_t ZP0(cpu6502 *cpu);
uint8_t ZPX(cpu6502 *cpu);
uint8_t ZPY(cpu6502 *cpu);
uint8_t REL(cpu6502 *cpu);
uint8_t ABS(cpu6502 *cpu);
uint8_t ABX(cpu6502 *cpu);
uint8_t ABY(cpu6502 *cpu);
uint8_t IND(cpu6502 *cpu);
uint8_t IZX(cpu6502 *cpu);
uint8_t IZY(cpu6502 *cpu);
