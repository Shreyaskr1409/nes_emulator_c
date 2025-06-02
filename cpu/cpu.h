#pragma once

#include <stdint.h>
#include <stdbool.h>

struct Bus;

typedef struct cpu6502 {
    uint8_t a;
    uint8_t x;
    uint8_t y;
    uint8_t stkp;
    uint8_t pc;
    uint8_t status;

    uint8_t fetched;
    uint16_t temp;
    uint16_t addr_abs;
    uint16_t addr_rel;
    uint8_t opcode;
    uint8_t cycles;
    uint32_t clock_count;

    struct Bus *bus;
    struct INSTRUCTION *lookup; // INSTRUCTION is defined below
                                      // this lookup table will be an array
                                      // it will point to the first element of the array
} cpu6502;

typedef struct INSTRUCTION {
    char name[4]; // I can store 3 characters in this
                  // one is reserved for null terminator \0
                  // null terminator marks end of the string
    uint8_t (*operate)(cpu6502*); // function pointer which takes cpu6502*
    uint8_t (*addrmode)(cpu6502*);
    uint8_t cycles;
} INSTRUCTION;

enum CPU_FLAGS {
    C = (1 << 0),	// Carry Bit
    Z = (1 << 1),	// Zero
    I = (1 << 2),	// Disable Interrupts
    D = (1 << 3),	// Decimal Mode (unused in this implementation)
    B = (1 << 4),	// Break
    U = (1 << 5),	// Unused
    V = (1 << 6),	// Overflow
    N = (1 << 7),	// Negative
};

extern INSTRUCTION lookup[256];

void CpuInit(cpu6502 *cpu);
void CpuDestroy(cpu6502 *cpu);

void CpuReset(cpu6502 *cpu);
void CpuIRQ(cpu6502 *cpu);
void CpuNMI(cpu6502 *cpu);
void CpuClock(cpu6502 *cpu);

// this will send signal that execution has completed
// this is needed so that we do not have to give clock manually
bool CpuComplete(cpu6502 *cpu);
void CpuConnectBus(cpu6502 *cpu, struct Bus *bus);

// used to access status register
uint8_t CpuGetFlag(cpu6502 *cpu, enum CPU_FLAGS f);
void CpuSetFlag(cpu6502 *cpu, enum CPU_FLAGS f, bool v);

uint8_t CpuReadFromBus(cpu6502 *cpu, uint16_t addr);
void CpuWriteFromBus(cpu6502 *cpu, uint16_t addr, uint8_t data);
uint8_t CpuFetchFromBus(cpu6502 *cpu);
