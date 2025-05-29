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
    struct INSTRUCTION *lookup_table; // INSTRUCTION is defined below
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
void CpuConnectBus(cpu6502 *cpu, struct Bus *n);

// used to access status register
uint8_t CpuGetFlag(cpu6502 *cpu, enum CPU_FLAGS f);
void CpuSetFLag(cpu6502 *cpu, enum CPU_FLAGS f, bool v);

uint8_t CpuRead(cpu6502 *cpu, uint16_t a);
void CpuWrite(cpu6502 *cpu, uint16_t a, uint8_t d);
uint8_t CpuFetch(cpu6502 *cpu);

uint8_t CpuAdrIMP(cpu6502 *cpu);
uint8_t CpuAdrIMM(cpu6502 *cpu);
uint8_t CpuAdrZP0(cpu6502 *cpu);
uint8_t CpuAdrZPX(cpu6502 *cpu);
uint8_t CpuAdrZPY(cpu6502 *cpu);
uint8_t CpuAdrREL(cpu6502 *cpu);
uint8_t CpuAdrABS(cpu6502 *cpu);
uint8_t CpuAdrABX(cpu6502 *cpu);
uint8_t CpuAdrABY(cpu6502 *cpu);
uint8_t CpuAdrIND(cpu6502 *cpu);
uint8_t CpuAdrIZX(cpu6502 *cpu);
uint8_t CpuAdrIZY(cpu6502 *cpu);

uint8_t CpuInsADC(cpu6502 *cpu);
uint8_t CpuInsAND(cpu6502 *cpu);
uint8_t CpuInsASL(cpu6502 *cpu);
uint8_t CpuInsBCC(cpu6502 *cpu);
uint8_t CpuInsBCS(cpu6502 *cpu);
uint8_t CpuInsBEQ(cpu6502 *cpu);
uint8_t CpuInsBIT(cpu6502 *cpu);
uint8_t CpuInsBMI(cpu6502 *cpu);
uint8_t CpuInsBNE(cpu6502 *cpu);
uint8_t CpuInsBPL(cpu6502 *cpu);
uint8_t CpuInsBRK(cpu6502 *cpu);
uint8_t CpuInsBVC(cpu6502 *cpu);
uint8_t CpuInsBVS(cpu6502 *cpu);
uint8_t CpuInsCLC(cpu6502 *cpu);
uint8_t CpuInsCLD(cpu6502 *cpu);
uint8_t CpuInsCLI(cpu6502 *cpu);
uint8_t CpuInsCLV(cpu6502 *cpu);
uint8_t CpuInsCMP(cpu6502 *cpu);
uint8_t CpuInsCPX(cpu6502 *cpu);
uint8_t CpuInsCPY(cpu6502 *cpu);
uint8_t CpuInsDEC(cpu6502 *cpu);
uint8_t CpuInsDEX(cpu6502 *cpu);
uint8_t CpuInsDEY(cpu6502 *cpu);
uint8_t CpuInsEOR(cpu6502 *cpu);
uint8_t CpuInsINC(cpu6502 *cpu);
uint8_t CpuInsINX(cpu6502 *cpu);
uint8_t CpuInsINY(cpu6502 *cpu);
uint8_t CpuInsJMP(cpu6502 *cpu);
uint8_t CpuInsJSR(cpu6502 *cpu);
uint8_t CpuInsLDA(cpu6502 *cpu);
uint8_t CpuInsLDX(cpu6502 *cpu);
uint8_t CpuInsLDY(cpu6502 *cpu);
uint8_t CpuInsLSR(cpu6502 *cpu);
uint8_t CpuInsNOP(cpu6502 *cpu);
uint8_t CpuInsORA(cpu6502 *cpu);
uint8_t CpuInsPHA(cpu6502 *cpu);
uint8_t CpuInsPHP(cpu6502 *cpu);
uint8_t CpuInsPLA(cpu6502 *cpu);
uint8_t CpuInsPLP(cpu6502 *cpu);
uint8_t CpuInsROL(cpu6502 *cpu);
uint8_t CpuInsROR(cpu6502 *cpu);
uint8_t CpuInsRTI(cpu6502 *cpu);
uint8_t CpuInsRTS(cpu6502 *cpu);
uint8_t CpuInsSBC(cpu6502 *cpu);
uint8_t CpuInsSEC(cpu6502 *cpu);
uint8_t CpuInsSED(cpu6502 *cpu);
uint8_t CpuInsSEI(cpu6502 *cpu);
uint8_t CpuInsSTA(cpu6502 *cpu);
uint8_t CpuInsSTX(cpu6502 *cpu);
uint8_t CpuInsSTY(cpu6502 *cpu);
uint8_t CpuInsTAX(cpu6502 *cpu);
uint8_t CpuInsTAY(cpu6502 *cpu);
uint8_t CpuInsTSX(cpu6502 *cpu);
uint8_t CpuInsTXA(cpu6502 *cpu);
uint8_t CpuInsTXS(cpu6502 *cpu);
uint8_t CpuInsTYA(cpu6502 *cpu);

uint8_t CpuInsXXX(cpu6502 *cpu);




















