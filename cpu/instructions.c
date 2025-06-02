#include "instructions.h"
#include "cpu.h"
#include <stdint.h>

// AND ACCUMULATOR
uint8_t AND(cpu6502 *cpu) {
    CpuFetchFromBus(cpu);
    cpu->a &= cpu->fetched;

    CpuSetFlag(cpu, Z, cpu->a==0x00);
    CpuSetFlag(cpu, N, cpu->a&0x80);
    return 1;
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
