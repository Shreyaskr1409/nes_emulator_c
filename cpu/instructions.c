#include "instructions.h"
#include "addressing_modes.h"
#include "cpu.h"
#include <stdint.h>

// ADD WITH CARRY
// Here we need to handle overflow cases and negative cases
// A = Acc. most significant bit (+ve or -ve)
// B = second number
// R = result
// V = overflow flag
// TRUTH TABLE:
// A M R | V
// 0 0 0 | 0
// 0 0 1 | 1
// 0 1 0 | 0
// 0 1 1 | 0
// 1 0 0 | 0
// 1 0 1 | 0
// 1 1 0 | 1
// 1 1 1 | 0
// upon making K-Maps or boolean algebra, we have V = ~(A^M)&(A^R)
uint8_t ADC(cpu6502 *cpu) {
    CpuFetchFromBus(cpu);
    cpu->temp = (uint16_t)cpu->a + (uint16_t)cpu->fetched + (uint16_t)CpuGetFlag(cpu, C);
    CpuSetFlag(cpu, C, cpu->temp > 255);
    CpuSetFlag(cpu, Z, (cpu->temp & 0x00FF) == 0);
    CpuSetFlag(cpu, N, (cpu->temp & 0x80));
    CpuSetFlag(cpu, V, (~((uint16_t)cpu->a ^ (uint16_t)cpu->fetched)
                & ((uint16_t)cpu->a ^ (uint16_t)cpu->temp)) & 0x0080);
    cpu->a = cpu->temp & 0x00FF;
    return 1;
}

// AND ACCUMULATOR
uint8_t AND(cpu6502 *cpu) {
    CpuFetchFromBus(cpu);
    cpu->a &= cpu->fetched;

    CpuSetFlag(cpu, Z, cpu->a==0x00);
    CpuSetFlag(cpu, N, cpu->a&0x80);
    return 1;
}

uint8_t ASL(cpu6502 *cpu) {
    CpuFetchFromBus(cpu);
    cpu->temp = cpu->fetched>>1;
    CpuSetFlag(cpu, C, (cpu->temp & 0xFF00) > 0);
    CpuSetFlag(cpu, N, cpu->temp == 0x80);
    CpuSetFlag(cpu, Z, (cpu->temp & 0x00FF) == 0x00);

    if (cpu->lookup[cpu->opcode].addrmode == IMP) {
        cpu->a = cpu->temp & 0x00FF;
    } else {
        CpuWriteFromBus(cpu, cpu->addr_abs, cpu->temp & 0x00FF);
    }
    return 0;
}

// BRANCH IF CARRY CLEAR
// adds 1 additional clock cycle if branching is in same page
// else adds 2 additional clock cycle (Given in the datasheet)
uint8_t BCC(cpu6502 *cpu) {
    if (CpuGetFlag(cpu, C) == 0) {
        cpu->cycles++;
        cpu->addr_abs = cpu->pc + cpu->addr_rel;

        if ((cpu->addr_abs&0xFF00) != (cpu->pc&0xFF00)) {
            cpu->cycles++;
        }

        cpu->pc = cpu->addr_abs;
    }
    return 0;
}

// BRANCH IF CARRY SET
// adds 1 additional clock cycle if branching is in same page
// else adds 2 additional clock cycle (Given in the datasheet)
uint8_t BCS(cpu6502 *cpu) {
    if (CpuGetFlag(cpu, C) == 1) {
        cpu->cycles++;
        cpu->addr_abs = cpu->pc + cpu->addr_rel;

        if ((cpu->addr_abs&0xFF00) != (cpu->pc&0xFF00)) {
            cpu->cycles++;
        }

        cpu->pc = cpu->addr_abs;
    }
    return 0;
}

uint8_t BEQ(cpu6502 *cpu) {
    if (CpuGetFlag(cpu, Z) == 1) {
        cpu->cycles++;
        cpu->addr_abs = cpu->pc + cpu->addr_rel;

        if ((cpu->addr_abs&0xFF00) != (cpu->pc&0xFF00)) {
            cpu->cycles++;
        }

        cpu->pc = cpu->addr_abs;
    }
    return 0;
}

uint8_t BIT(cpu6502 *cpu) {
    CpuFetchFromBus(cpu);
    cpu->temp = cpu->a & cpu->fetched;
    CpuSetFlag(cpu, Z, (cpu->temp & 0x00FF) == 0x0000);
    CpuSetFlag(cpu, V, cpu->temp & (1<<6));
    CpuSetFlag(cpu, N, cpu->temp & (1<<7));
    return 0;
}

uint8_t BMI(cpu6502 *cpu) {
    if (CpuGetFlag(cpu, N) == 1) {
        cpu->cycles++;
        cpu->addr_abs = cpu->pc + cpu->addr_rel;

        if ((cpu->addr_abs&0xFF00) != (cpu->pc&0xFF00)) {
            cpu->cycles++;
        }

        cpu->pc = cpu->addr_abs;
    }
    return 0;
}

uint8_t BNE(cpu6502 *cpu) {
    if (CpuGetFlag(cpu, Z) == 0) {
        cpu->cycles++;
        cpu->addr_abs = cpu->pc + cpu->addr_rel;

        if ((cpu->addr_abs&0xFF00) != (cpu->pc&0xFF00)) {
            cpu->cycles++;
        }

        cpu->pc = cpu->addr_abs;
    }
    return 0;
}

uint8_t BPL(cpu6502 *cpu) {
    if (CpuGetFlag(cpu, N) == 0) {
        cpu->cycles++;
        cpu->addr_abs = cpu->pc + cpu->addr_rel;

        if ((cpu->addr_abs&0xFF00) != (cpu->pc&0xFF00)) {
            cpu->cycles++;
        }

        cpu->pc = cpu->addr_abs;
    }
    return 0;
}

uint8_t BRK(cpu6502 *cpu) {
    // Set interrupt as ON before doing anything
    CpuSetFlag(cpu, I, true);
    CpuWriteFromBus(cpu, 0x0100 + cpu->stkp, (cpu->pc >> 8) & 0x00FF);
    cpu->stkp--;
    CpuWriteFromBus(cpu, 0x0100 + cpu->stkp, cpu->pc & 0x00FF);
    cpu->stkp--;

    // Storing flags to the stkp so that whenever someone jumps
    // to the location in the memory, it knows the flags which were
    // present at the time when BRK was executed
    CpuSetFlag(cpu, B, true);
    CpuWriteFromBus(cpu, cpu->stkp, cpu->status);
    cpu->stkp--;
    CpuSetFlag(cpu, B, false);

    return 0;
}

uint8_t BVC(cpu6502 *cpu) {
    if (CpuGetFlag(cpu, V) == 0) {
        cpu->cycles++;
        cpu->addr_abs = cpu->pc + cpu->addr_rel;

        if ((cpu->addr_abs&0xFF00) != (cpu->pc&0xFF00)) {
            cpu->cycles++;
        }

        cpu->pc = cpu->addr_abs;
    }
    return 0;
}

uint8_t BVS(cpu6502 *cpu) {
    if (CpuGetFlag(cpu, V) == 1) {
        cpu->cycles++;
        cpu->addr_abs = cpu->pc + cpu->addr_rel;

        if ((cpu->addr_abs&0xFF00) != (cpu->pc&0xFF00)) {
            cpu->cycles++;
        }

        cpu->pc = cpu->addr_abs;
    }
    return 0;
}

uint8_t CLC(cpu6502 *cpu) {
    CpuSetFlag(cpu, C, false);
    return 0;
}
uint8_t CLD(cpu6502 *cpu) {
    CpuSetFlag(cpu, D, false);
    return 0;
}
uint8_t CLI(cpu6502 *cpu) {
    CpuSetFlag(cpu, I, false);
    return 0;
}
uint8_t CLV(cpu6502 *cpu) {
    CpuSetFlag(cpu, I, false);
    return 0;
}

uint8_t CMP(cpu6502 *cpu) {
    CpuFetchFromBus(cpu);
    cpu->temp = cpu->a - cpu->fetched;
    CpuSetFlag(cpu, C, cpu->a >= cpu->fetched);
    CpuSetFlag(cpu, Z, cpu->a == cpu->fetched);
    CpuSetFlag(cpu, N, cpu->temp & (1<<7));
    return 1;
}

uint8_t CMX(cpu6502 *cpu) {
    CpuFetchFromBus(cpu);
    cpu->temp = cpu->x - cpu->fetched;
    CpuSetFlag(cpu, C, cpu->x >= cpu->fetched);
    CpuSetFlag(cpu, Z, cpu->x == cpu->fetched);
    CpuSetFlag(cpu, N, cpu->temp & (1<<7));
    return 1;
}

uint8_t CMY(cpu6502 *cpu) {
    CpuFetchFromBus(cpu);
    cpu->temp = cpu->y - cpu->fetched;
    CpuSetFlag(cpu, C, cpu->y >= cpu->fetched);
    CpuSetFlag(cpu, Z, cpu->y == cpu->fetched);
    CpuSetFlag(cpu, N, cpu->temp & (1<<7));
    return 1;
}

uint8_t DEC(cpu6502 *cpu) {
    CpuFetchFromBus(cpu);
    cpu->temp = cpu->fetched - 1;
    CpuSetFlag(cpu, Z, (cpu->temp & 0x00FF) == 0);
    CpuSetFlag(cpu, N, cpu->temp & (1<<7));
    CpuWriteFromBus(cpu, cpu->addr_abs, cpu->temp);
    return 0;
}

uint8_t DEX(cpu6502 *cpu) {
    cpu->x--;
    CpuSetFlag(cpu, Z, cpu->x==0);
    CpuSetFlag(cpu, N, cpu->x & (1<<7));
    return 0;
}

uint8_t DEY(cpu6502 *cpu) {
    cpu->y--;
    CpuSetFlag(cpu, Z, cpu->y==0);
    CpuSetFlag(cpu, N, cpu->y & (1<<7));
    return 0;
}

// Put into stack from accumulator
uint8_t PHA(cpu6502 *cpu) {
    // Stackpointer is always an offset to the memory location of 0x0100
    CpuWriteFromBus(cpu, 0x0100 + cpu->stkp, cpu->a);
    cpu->stkp--;
    return 0;
}

// Take out from stack into accumulator
uint8_t PLA(cpu6502 *cpu) {
    cpu->stkp++;
    cpu->a = CpuReadFromBus(cpu, 0x0100 + cpu->stkp);
    CpuSetFlag(cpu, Z, cpu->a==0x00);
    CpuSetFlag(cpu, N, cpu->a&0x80);
    return 0;
}

// SUBTRACT WITH CARRY (Borrow)
uint8_t SBC(cpu6502 *cpu) {
    CpuFetchFromBus(cpu);
    uint16_t value = ((uint16_t)cpu->fetched) ^ 0x00FF;

    cpu->temp = (uint16_t)cpu->a + value + (uint16_t)CpuGetFlag(cpu, C);
    CpuSetFlag(cpu, C, cpu->temp & 0xFF00);
    CpuSetFlag(cpu, Z, (cpu->temp & 0x00FF) == 0);
    CpuSetFlag(cpu, N, (cpu->temp & 0x80));
    CpuSetFlag(cpu, V, (((uint16_t)cpu->a ^ value)
                & ((uint16_t)cpu->a ^ (uint16_t)cpu->temp)) & 0x0080);
    cpu->a = cpu->temp & 0x00FF;
    return 1;
}


uint8_t SEC(cpu6502 *cpu) {
    CpuSetFlag(cpu, C, true);
    return 0;
}
uint8_t SED(cpu6502 *cpu) {
    CpuSetFlag(cpu, D, true);
    return 0;
}
uint8_t SEI(cpu6502 *cpu) {
    CpuSetFlag(cpu, I, true);
    return 0;
}

uint8_t STA(cpu6502 *cpu) {
    CpuWriteFromBus(cpu, cpu->addr_abs, cpu->a);
    return 0;
}

uint8_t STX(cpu6502 *cpu) {
    CpuWriteFromBus(cpu, cpu->addr_abs, cpu->x);
    return 0;
}

uint8_t STY(cpu6502 *cpu) {
    CpuWriteFromBus(cpu, cpu->addr_abs, cpu->y);
    return 0;
}

uint8_t TAX(cpu6502 *cpu) {
    cpu->x = cpu->a;
    CpuSetFlag(cpu, Z, cpu->x == 0x00);
    CpuSetFlag(cpu, N, cpu->x & (1<<7));
    return 0;
}

uint8_t TAY(cpu6502 *cpu) {
    cpu->y = cpu->a;
    CpuSetFlag(cpu, Z, cpu->y == 0x00);
    CpuSetFlag(cpu, N, cpu->y & (1<<7));
    return 0;
}

uint8_t TSX(cpu6502 *cpu) {
    cpu->x = cpu->stkp;
    CpuSetFlag(cpu, Z, cpu->x == 0x00);
    CpuSetFlag(cpu, N, cpu->x & (1<<7));
    return 0;
}

uint8_t TXA(cpu6502 *cpu) {
    cpu->a = cpu->x;
    CpuSetFlag(cpu, Z, cpu->a == 0x00);
    CpuSetFlag(cpu, N, cpu->a & (1<<7));
    return 0;
}

uint8_t TSY(cpu6502 *cpu) {
    cpu->y = cpu->stkp;
    CpuSetFlag(cpu, Z, cpu->y == 0x00);
    CpuSetFlag(cpu, N, cpu->y & (1<<7));
    return 0;
}

uint8_t TYA(cpu6502 *cpu) {
    cpu->a = cpu->y;
    CpuSetFlag(cpu, Z, cpu->a == 0x00);
    CpuSetFlag(cpu, N, cpu->a & (1<<7));
    return 0;
}

