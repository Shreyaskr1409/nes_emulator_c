#include "addressing_modes.h"
#include "cpu.h"
#include <stdint.h>

// IMPLIED
// This means that there is no data provided explicitly in the instruction
// Things like CMA (Compliment accumulator), RAL (Rotate accumulator left)
// use IMPLIED addressing mode
uint8_t IMP(cpu6502 *cpu) {
    // data might be in the accumulator
    cpu->fetched = cpu->a;
    return 0x00;
}

// IMMEDIATE
// This means that the data is provided immediately after the Operation's
// bit representation.
uint8_t IMM(cpu6502 *cpu) {
    // data is in the program counter
    cpu->addr_abs = cpu->pc;
    cpu->pc++;
    return 0x00;
}

// Memory address can be of 16 bits
// Those can be represented as 0xABCD
// Here, AB bits represent the page no.
// CD bits represent the offset inside the page
// In short, it is like line number inside a page in a normal textbook

// ZERO PAGE ADDRESSING
// Uses 0th page to address data
// Not to be confused with Implied as it looks similar in assembly as:
// OPCODE #$DATA -> IMP
// OPCODE $0092 -> ZP0 ($0092 is yet an address or just an index in the 
// zero page)
uint8_t ZP0(cpu6502 *cpu) {
    // Data is somewhere in the page 0
    // addr_abs here is storing address of the data by copying the
    // offset inside the page
    cpu->addr_abs = CpuReadFromBus(cpu, cpu->pc) & 0x00FF;
    cpu->pc++;
    return 0;
}

// Same as ZP0 but now there is an additional offset of X
uint8_t ZPX(cpu6502 *cpu) {
    cpu->addr_abs = (CpuReadFromBus(cpu, cpu->pc) + cpu->x) & 0x00FF;
    cpu->pc++;
    return 0;
}

// Same as ZP0 but now there is an additional offset of Y
uint8_t ZPY(cpu6502 *cpu) {
    cpu->addr_abs = (CpuReadFromBus(cpu, cpu->pc) + cpu->y) & 0x00FF;
    cpu->pc++;
    return 0;
}

// 3 byte instruction which has last two bytes as the address itself
uint8_t ABS(cpu6502 *cpu) {
    uint16_t lo = CpuReadFromBus(cpu, cpu->pc);
    cpu->pc++;
    uint16_t hi = CpuReadFromBus(cpu, cpu->pc);
    cpu->pc++;

    cpu->addr_abs = (hi << 8) | lo;

    return 0;
}

uint8_t ABX(cpu6502 *cpu) {
    uint16_t lo = CpuReadFromBus(cpu, cpu->pc);
    cpu->pc++;
    uint16_t hi = CpuReadFromBus(cpu, cpu->pc);
    cpu->pc++;

    cpu->addr_abs = (hi << 8) | lo + cpu->x;

    if ((cpu->addr_abs>>8) != hi) {
        return 1;
    }
    return 0;
}

uint8_t ABY(cpu6502 *cpu) {
    uint16_t lo = CpuReadFromBus(cpu, cpu->pc);
    cpu->pc++;
    uint16_t hi = CpuReadFromBus(cpu, cpu->pc);
    cpu->pc++;

    cpu->addr_abs = (hi << 8) | lo + cpu->y;

    if ((cpu->addr_abs>>8) != hi) {
        return 1;
    }
    return 0;
}

// INDIRECT
// This is 6502's way to implement pointers
// You basically are providing address of the address of the data in
// the operation
uint8_t IND(cpu6502 *cpu) {
    uint16_t ptr_lo = CpuReadFromBus(cpu, cpu->pc);
    cpu->pc++;
    uint16_t ptr_hi = CpuReadFromBus(cpu, cpu->pc);
    cpu->pc++;

    uint16_t ptr = (ptr_hi<<8) | ptr_lo;

    if (ptr_lo == 0x00FF) {
        cpu->addr_abs = (CpuReadFromBus(cpu, ptr&0xFF00)<<8) | CpuReadFromBus(cpu, ptr);
    } else {
        cpu->addr_abs = (CpuReadFromBus(cpu, ptr+1)<<8) | CpuReadFromBus(cpu, ptr);
    }

    return 0;
}

// INDIRECT X
// This is like 0th page certain 8-bit index + x offset
// this position contains an address. That address is a pointer to the
// original data
uint8_t IZX(cpu6502 *cpu) {
    uint16_t t = CpuReadFromBus(cpu, cpu->pc);
    cpu->pc++;

    uint16_t lo = CpuReadFromBus(cpu, (uint16_t)(t + (uint16_t)cpu->x) & 0x00FF);
    uint16_t hi = CpuReadFromBus(cpu, (uint16_t)(t + (uint16_t)cpu->x + 1) & 0x00FF);

    cpu->addr_abs = (hi<<8) | lo;

    return 0;
}

// INDIRECT Y
// Y offset is provided to the pointer
uint8_t IZY(cpu6502 *cpu) {
    uint16_t t = CpuReadFromBus(cpu, cpu->pc);
    cpu->pc++;

    uint16_t lo = CpuReadFromBus(cpu, t & 0x00FF);
    uint16_t hi = CpuReadFromBus(cpu, (t+1) & 0x00FF);

    cpu->addr_abs = (hi<<8) | lo;
    cpu->addr_abs += cpu->y;

    if ((cpu->addr_abs>>8) != hi) {
        return 1;
    }
    return 0;
}

uint8_t REL(cpu6502 *cpu) {
    cpu->addr_rel = CpuReadFromBus(cpu, cpu->pc);
    cpu->pc++;
    if (cpu->addr_rel & 0x80) {
        cpu->addr_rel |=0xFF00;
    }
    return 0;
}
