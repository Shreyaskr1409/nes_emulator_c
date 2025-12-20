#include "cpu.h"
#include "../bus/bus.h"
#include "addressing_modes.h"
#include "instructions.h"
#include "stc/cstr.h"

#include <stdbool.h>
#include <stdint.h>

// this pattern of lookup table is taken from OneLoneCoder's NES emulator
// 16x16 lookup table
INSTRUCTION lookup[256] = {
    { "BRK", &BRK, &IMM, 7 },{ "ORA", &ORA, &IZX, 6 },{ "???", &XXX, &IMP, 2 },{ "???", &XXX, &IMP, 8 },{ "???", &NOP, &IMP, 3 },{ "ORA", &ORA, &ZP0, 3 },{ "ASL", &ASL, &ZP0, 5 },{ "???", &XXX, &IMP, 5 },{ "PHP", &PHP, &IMP, 3 },{ "ORA", &ORA, &IMM, 2 },{ "ASL", &ASL, &IMP, 2 },{ "???", &XXX, &IMP, 2 },{ "???", &NOP, &IMP, 4 },{ "ORA", &ORA, &ABS, 4 },{ "ASL", &ASL, &ABS, 6 },{ "???", &XXX, &IMP, 6 },
    { "BPL", &BPL, &REL, 2 },{ "ORA", &ORA, &IZY, 5 },{ "???", &XXX, &IMP, 2 },{ "???", &XXX, &IMP, 8 },{ "???", &NOP, &IMP, 4 },{ "ORA", &ORA, &ZPX, 4 },{ "ASL", &ASL, &ZPX, 6 },{ "???", &XXX, &IMP, 6 },{ "CLC", &CLC, &IMP, 2 },{ "ORA", &ORA, &ABY, 4 },{ "???", &NOP, &IMP, 2 },{ "???", &XXX, &IMP, 7 },{ "???", &NOP, &IMP, 4 },{ "ORA", &ORA, &ABX, 4 },{ "ASL", &ASL, &ABX, 7 },{ "???", &XXX, &IMP, 7 },
    { "JSR", &JSR, &ABS, 6 },{ "AND", &AND, &IZX, 6 },{ "???", &XXX, &IMP, 2 },{ "???", &XXX, &IMP, 8 },{ "BIT", &BIT, &ZP0, 3 },{ "AND", &AND, &ZP0, 3 },{ "ROL", &ROL, &ZP0, 5 },{ "???", &XXX, &IMP, 5 },{ "PLP", &PLP, &IMP, 4 },{ "AND", &AND, &IMM, 2 },{ "ROL", &ROL, &IMP, 2 },{ "???", &XXX, &IMP, 2 },{ "BIT", &BIT, &ABS, 4 },{ "AND", &AND, &ABS, 4 },{ "ROL", &ROL, &ABS, 6 },{ "???", &XXX, &IMP, 6 },
    { "BMI", &BMI, &REL, 2 },{ "AND", &AND, &IZY, 5 },{ "???", &XXX, &IMP, 2 },{ "???", &XXX, &IMP, 8 },{ "???", &NOP, &IMP, 4 },{ "AND", &AND, &ZPX, 4 },{ "ROL", &ROL, &ZPX, 6 },{ "???", &XXX, &IMP, 6 },{ "SEC", &SEC, &IMP, 2 },{ "AND", &AND, &ABY, 4 },{ "???", &NOP, &IMP, 2 },{ "???", &XXX, &IMP, 7 },{ "???", &NOP, &IMP, 4 },{ "AND", &AND, &ABX, 4 },{ "ROL", &ROL, &ABX, 7 },{ "???", &XXX, &IMP, 7 },
    { "RTI", &RTI, &IMP, 6 },{ "EOR", &EOR, &IZX, 6 },{ "???", &XXX, &IMP, 2 },{ "???", &XXX, &IMP, 8 },{ "???", &NOP, &IMP, 3 },{ "EOR", &EOR, &ZP0, 3 },{ "LSR", &LSR, &ZP0, 5 },{ "???", &XXX, &IMP, 5 },{ "PHA", &PHA, &IMP, 3 },{ "EOR", &EOR, &IMM, 2 },{ "LSR", &LSR, &IMP, 2 },{ "???", &XXX, &IMP, 2 },{ "JMP", &JMP, &ABS, 3 },{ "EOR", &EOR, &ABS, 4 },{ "LSR", &LSR, &ABS, 6 },{ "???", &XXX, &IMP, 6 },
    { "BVC", &BVC, &REL, 2 },{ "EOR", &EOR, &IZY, 5 },{ "???", &XXX, &IMP, 2 },{ "???", &XXX, &IMP, 8 },{ "???", &NOP, &IMP, 4 },{ "EOR", &EOR, &ZPX, 4 },{ "LSR", &LSR, &ZPX, 6 },{ "???", &XXX, &IMP, 6 },{ "CLI", &CLI, &IMP, 2 },{ "EOR", &EOR, &ABY, 4 },{ "???", &NOP, &IMP, 2 },{ "???", &XXX, &IMP, 7 },{ "???", &NOP, &IMP, 4 },{ "EOR", &EOR, &ABX, 4 },{ "LSR", &LSR, &ABX, 7 },{ "???", &XXX, &IMP, 7 },
    { "RTS", &RTS, &IMP, 6 },{ "ADC", &ADC, &IZX, 6 },{ "???", &XXX, &IMP, 2 },{ "???", &XXX, &IMP, 8 },{ "???", &NOP, &IMP, 3 },{ "ADC", &ADC, &ZP0, 3 },{ "ROR", &ROR, &ZP0, 5 },{ "???", &XXX, &IMP, 5 },{ "PLA", &PLA, &IMP, 4 },{ "ADC", &ADC, &IMM, 2 },{ "ROR", &ROR, &IMP, 2 },{ "???", &XXX, &IMP, 2 },{ "JMP", &JMP, &IND, 5 },{ "ADC", &ADC, &ABS, 4 },{ "ROR", &ROR, &ABS, 6 },{ "???", &XXX, &IMP, 6 },
    { "BVS", &BVS, &REL, 2 },{ "ADC", &ADC, &IZY, 5 },{ "???", &XXX, &IMP, 2 },{ "???", &XXX, &IMP, 8 },{ "???", &NOP, &IMP, 4 },{ "ADC", &ADC, &ZPX, 4 },{ "ROR", &ROR, &ZPX, 6 },{ "???", &XXX, &IMP, 6 },{ "SEI", &SEI, &IMP, 2 },{ "ADC", &ADC, &ABY, 4 },{ "???", &NOP, &IMP, 2 },{ "???", &XXX, &IMP, 7 },{ "???", &NOP, &IMP, 4 },{ "ADC", &ADC, &ABX, 4 },{ "ROR", &ROR, &ABX, 7 },{ "???", &XXX, &IMP, 7 },
    { "???", &NOP, &IMP, 2 },{ "STA", &STA, &IZX, 6 },{ "???", &NOP, &IMP, 2 },{ "???", &XXX, &IMP, 6 },{ "STY", &STY, &ZP0, 3 },{ "STA", &STA, &ZP0, 3 },{ "STX", &STX, &ZP0, 3 },{ "???", &XXX, &IMP, 3 },{ "DEY", &DEY, &IMP, 2 },{ "???", &NOP, &IMP, 2 },{ "TXA", &TXA, &IMP, 2 },{ "???", &XXX, &IMP, 2 },{ "STY", &STY, &ABS, 4 },{ "STA", &STA, &ABS, 4 },{ "STX", &STX, &ABS, 4 },{ "???", &XXX, &IMP, 4 },
    { "BCC", &BCC, &REL, 2 },{ "STA", &STA, &IZY, 6 },{ "???", &XXX, &IMP, 2 },{ "???", &XXX, &IMP, 6 },{ "STY", &STY, &ZPX, 4 },{ "STA", &STA, &ZPX, 4 },{ "STX", &STX, &ZPY, 4 },{ "???", &XXX, &IMP, 4 },{ "TYA", &TYA, &IMP, 2 },{ "STA", &STA, &ABY, 5 },{ "TXS", &TXS, &IMP, 2 },{ "???", &XXX, &IMP, 5 },{ "???", &NOP, &IMP, 5 },{ "STA", &STA, &ABX, 5 },{ "???", &XXX, &IMP, 5 },{ "???", &XXX, &IMP, 5 },
    { "LDY", &LDY, &IMM, 2 },{ "LDA", &LDA, &IZX, 6 },{ "LDX", &LDX, &IMM, 2 },{ "???", &XXX, &IMP, 6 },{ "LDY", &LDY, &ZP0, 3 },{ "LDA", &LDA, &ZP0, 3 },{ "LDX", &LDX, &ZP0, 3 },{ "???", &XXX, &IMP, 3 },{ "TAY", &TAY, &IMP, 2 },{ "LDA", &LDA, &IMM, 2 },{ "TAX", &TAX, &IMP, 2 },{ "???", &XXX, &IMP, 2 },{ "LDY", &LDY, &ABS, 4 },{ "LDA", &LDA, &ABS, 4 },{ "LDX", &LDX, &ABS, 4 },{ "???", &XXX, &IMP, 4 },
    { "BCS", &BCS, &REL, 2 },{ "LDA", &LDA, &IZY, 5 },{ "???", &XXX, &IMP, 2 },{ "???", &XXX, &IMP, 5 },{ "LDY", &LDY, &ZPX, 4 },{ "LDA", &LDA, &ZPX, 4 },{ "LDX", &LDX, &ZPY, 4 },{ "???", &XXX, &IMP, 4 },{ "CLV", &CLV, &IMP, 2 },{ "LDA", &LDA, &ABY, 4 },{ "TSX", &TSX, &IMP, 2 },{ "???", &XXX, &IMP, 4 },{ "LDY", &LDY, &ABX, 4 },{ "LDA", &LDA, &ABX, 4 },{ "LDX", &LDX, &ABY, 4 },{ "???", &XXX, &IMP, 4 },
    { "CPY", &CPY, &IMM, 2 },{ "CMP", &CMP, &IZX, 6 },{ "???", &NOP, &IMP, 2 },{ "???", &XXX, &IMP, 8 },{ "CPY", &CPY, &ZP0, 3 },{ "CMP", &CMP, &ZP0, 3 },{ "DEC", &DEC, &ZP0, 5 },{ "???", &XXX, &IMP, 5 },{ "INY", &INY, &IMP, 2 },{ "CMP", &CMP, &IMM, 2 },{ "DEX", &DEX, &IMP, 2 },{ "???", &XXX, &IMP, 2 },{ "CPY", &CPY, &ABS, 4 },{ "CMP", &CMP, &ABS, 4 },{ "DEC", &DEC, &ABS, 6 },{ "???", &XXX, &IMP, 6 },
    { "BNE", &BNE, &REL, 2 },{ "CMP", &CMP, &IZY, 5 },{ "???", &XXX, &IMP, 2 },{ "???", &XXX, &IMP, 8 },{ "???", &NOP, &IMP, 4 },{ "CMP", &CMP, &ZPX, 4 },{ "DEC", &DEC, &ZPX, 6 },{ "???", &XXX, &IMP, 6 },{ "CLD", &CLD, &IMP, 2 },{ "CMP", &CMP, &ABY, 4 },{ "NOP", &NOP, &IMP, 2 },{ "???", &XXX, &IMP, 7 },{ "???", &NOP, &IMP, 4 },{ "CMP", &CMP, &ABX, 4 },{ "DEC", &DEC, &ABX, 7 },{ "???", &XXX, &IMP, 7 },
    { "CPX", &CPX, &IMM, 2 },{ "SBC", &SBC, &IZX, 6 },{ "???", &NOP, &IMP, 2 },{ "???", &XXX, &IMP, 8 },{ "CPX", &CPX, &ZP0, 3 },{ "SBC", &SBC, &ZP0, 3 },{ "INC", &INC, &ZP0, 5 },{ "???", &XXX, &IMP, 5 },{ "INX", &INX, &IMP, 2 },{ "SBC", &SBC, &IMM, 2 },{ "NOP", &NOP, &IMP, 2 },{ "???", &SBC, &IMP, 2 },{ "CPX", &CPX, &ABS, 4 },{ "SBC", &SBC, &ABS, 4 },{ "INC", &INC, &ABS, 6 },{ "???", &XXX, &IMP, 6 },
    { "BEQ", &BEQ, &REL, 2 },{ "SBC", &SBC, &IZY, 5 },{ "???", &XXX, &IMP, 2 },{ "???", &XXX, &IMP, 8 },{ "???", &NOP, &IMP, 4 },{ "SBC", &SBC, &ZPX, 4 },{ "INC", &INC, &ZPX, 6 },{ "???", &XXX, &IMP, 6 },{ "SED", &SED, &IMP, 2 },{ "SBC", &SBC, &ABY, 4 },{ "NOP", &NOP, &IMP, 2 },{ "???", &XXX, &IMP, 7 },{ "???", &NOP, &IMP, 4 },{ "SBC", &SBC, &ABX, 4 },{ "INC", &INC, &ABX, 7 },{ "???", &XXX, &IMP, 7 },
};

void CpuInit(cpu6502 *cpu) {
    // here we will initialize variables and then we can assemble the lookup table
    // C does not initialize local integers by itself to 0
    // even though I will be using global ones, I will initialize them to be safe
    cpu->a = 0x00;
    cpu->x = 0x00;
    cpu->y = 0x00;
    cpu->stkp = 0x00;
    cpu->pc = 0x0000;
    cpu->status = 0x00;

    cpu->fetched = 0x00;
    cpu->temp = 0x0000;
    cpu->addr_abs = 0x0000;
    cpu->addr_rel = 0x0000;
    cpu->opcode = 0x00;
    cpu->cycles = 0;
    cpu->clock_count = 0;

    cpu->lookup = lookup;
}

// For enabling Data to flow through the Bus:
uint8_t CpuReadFromBus(cpu6502 *cpu, uint16_t addr) {
    return BusRead(cpu->bus, addr, false);
}
void CpuWriteToCpuBus(cpu6502 *cpu, uint16_t addr, uint8_t data) {
    BusWrite(cpu->bus, addr, data);
}
uint8_t CpuFetchFromBus(cpu6502 *cpu) {
    if (!(cpu->lookup[cpu->opcode].addrmode == &IMP)) {
        cpu->fetched = CpuReadFromBus(cpu, cpu->addr_abs);
    }
    return cpu->fetched;
}




void CpuReset(cpu6502 *cpu) {
    cpu->addr_abs = 0xFFFC;
    uint8_t lo = CpuReadFromBus(cpu, cpu->addr_abs + 0);
    uint8_t hi = CpuReadFromBus(cpu, cpu->addr_abs + 1);

    cpu->pc = (hi<<8) | lo;

    cpu->a = 0;
    cpu->x = 0;
    cpu->y = 0;
    cpu->stkp = 0xFD;
    cpu->status = 0x00 | U;

    cpu->addr_rel = 0x0000;
    cpu->addr_abs = 0x0000;
    cpu->fetched = 0x00;

    cpu->cycles = 8;
}
void CpuIRQ(cpu6502 *cpu) {
    // if interrupt flag is 0, then we can perform the interrupt request (IRQ)
    if (CpuGetFlag(cpu, I) == 0) {
        CpuWriteToCpuBus(cpu, 0x0100 + cpu->stkp, (cpu->pc>>8) & 0x00FF);
        cpu->stkp--;
        CpuWriteToCpuBus(cpu, 0x0100 + cpu->stkp, (cpu->pc) & 0x00FF);
        cpu->stkp--;

        CpuSetFlag(cpu, B, 0);
        CpuSetFlag(cpu, U, 1);
        CpuSetFlag(cpu, I, 1);
        CpuWriteToCpuBus(cpu, 0x0100 + cpu->stkp, cpu->status);
        cpu->stkp--;

        cpu->addr_abs = 0xFFFE;
        // the low bit is saved before the high bit
        uint16_t lo = CpuReadFromBus(cpu, cpu->addr_abs + 0);
        uint16_t hi = CpuReadFromBus(cpu, cpu->addr_abs + 1);
        cpu->pc = (hi<<8)|lo;

        cpu->cycles = 7;
    }
}
void CpuNMI(cpu6502 *cpu) {
    // similar to IRQ but does not require I-flag to be 0 and pc is
    // set to address from 0xFFFA
    CpuWriteToCpuBus(cpu, 0x0100 + cpu->stkp, (cpu->pc>>8) & 0x00FF);
    cpu->stkp--;
    CpuWriteToCpuBus(cpu, 0x0100 + cpu->stkp, (cpu->pc) & 0x00FF);
    cpu->stkp--;

    CpuSetFlag(cpu, B, 0);
    CpuSetFlag(cpu, U, 1);
    CpuSetFlag(cpu, I, 1);
    CpuWriteToCpuBus(cpu, 0x0100 + cpu->stkp, cpu->status);
    cpu->stkp--;

    cpu->addr_abs = 0xFFFA;
    // the low bit is saved before the high bit
    uint16_t lo = CpuReadFromBus(cpu, cpu->addr_abs + 0);
    uint16_t hi = CpuReadFromBus(cpu, cpu->addr_abs + 1);
    cpu->pc = (hi<<8)|lo;

    cpu->cycles = 8;
}
void CpuClock(cpu6502 *cpu) {
    if (!cpu) return; // Add null check
                      //
    if (cpu->cycles == 0) {
        cpu->opcode = CpuReadFromBus(cpu, cpu->pc);

        // printf("----------------------------------------\n");
        // printf("PC: 0x%04X, Opcode: 0x%02X, Cycles: %d\n", 
        //        cpu->pc, cpu->opcode, cpu->lookup[cpu->opcode].cycles);
        // printf("Addrmode ptr: %p, Operate ptr: %p\n", 
        //        (void*)cpu->lookup[cpu->opcode].addrmode, 
        //        (void*)cpu->lookup[cpu->opcode].operate);

        cpu->pc++;

        cpu->cycles = cpu->lookup[cpu->opcode].cycles;
        uint8_t additional_cycles1 = (cpu->lookup[cpu->opcode].addrmode)(cpu);
        uint8_t additional_cycles2 = (cpu->lookup[cpu->opcode].operate)(cpu);
        cpu->cycles += additional_cycles1 & additional_cycles2;
        // additional_cycles could be only 1 or 0 in this use-case
        // thus & operation is being done, else it is better to find out the
        // maximum number of cycles required
    }
    cpu->cycles--;
}



uint8_t CpuGetFlag(cpu6502 *cpu, enum CPU_FLAGS f) {
    if ((cpu->status&f) > 0) {
        return 1;
    }
    return 0;
}
void CpuSetFlag(cpu6502 *cpu, enum CPU_FLAGS f, bool v) {
    if (v) {
        cpu->status |= f;
    } else {
        cpu->status &= ~f;
    }
}


bool CpuComplete(cpu6502 *cpu) {
    return cpu->cycles==0;
}
void CpuConnectBus(cpu6502 *cpu, struct Bus *bus) {
    cpu->bus = bus;
}


// ------------------DISASSEMBLER-------------------- //
// // utility to convert variables into hexadecimal strings
// static cstr hex(uint32_t n, uint8_t d) {
//     // string with d (digit count) and filled with char '0'
//     cstr s = cstr_with_size(d, '0');
//     char repl[2] = {0}; // Single char + null terminator
//
//     // shift by 16 bits at a time
//     for (int i = d-1; i >= 0; i--, n>>=4) {
//         repl[0] = "0123456789ABCDEF"[n&0xF];
//         cstr_replace_at(&s, i, 1, repl);
//     }
//
//     return s;
// }
// disassembler
hmap_uint16_str disassemble(cpu6502 *cpu, uint16_t nStart, uint16_t nStop) {
    uint32_t addr = nStart;
    uint8_t value = 0x00;
    uint8_t lo = 0x00;
    uint8_t hi = 0x00;
    hmap_uint16_str mapLines = {};
    uint16_t line_addr = 0;

    while (addr <= (uint32_t)nStop) {
        line_addr = addr;
        cstr sInst = cstr_from_fmt("$%0*X: ", 4, addr); // 0 is the padding of 0
                                                        // * takes the width from argument list
                                                        // X is hexadecimal in uppercase
        // read instruction and get its readable name
        uint8_t opcode = CpuReadFromBus(cpu, addr);
        addr++;
        cstr_append_fmt(&sInst, "%s ", cpu->lookup[opcode].name);

        if (cpu->lookup[opcode].addrmode == IMP) {
            cstr_append_fmt(&sInst, " {IMP}");
        } else if (cpu->lookup[opcode].addrmode == IMM) {
            value = CpuReadFromBus(cpu, addr); addr++;
            cstr_append_fmt(&sInst, "#$%02X {IMM}", value);
        } else if (cpu->lookup[opcode].addrmode == ZP0) {
            lo = CpuReadFromBus(cpu, addr); addr++;
            hi = 0x00;
            cstr_append_fmt(&sInst, "$%02X {ZP0}", lo);
        } else if (cpu->lookup[opcode].addrmode == ZPX) {
            lo = CpuReadFromBus(cpu, addr); addr++;
            hi = 0x00;
            cstr_append_fmt(&sInst, "$%02X, X {ZPX}", lo);
        } else if (cpu->lookup[opcode].addrmode == ZPY) {
            lo = CpuReadFromBus(cpu, addr); addr++;
            hi = 0x00;
            cstr_append_fmt(&sInst, "$%02X, Y {ZPY}", lo);
        } else if (cpu->lookup[opcode].addrmode == IZX) {
            lo = CpuReadFromBus(cpu, addr); addr++;
            hi = 0x00;
            cstr_append_fmt(&sInst, "($%02X, X) {IZX}", lo);
        } else if (cpu->lookup[opcode].addrmode == IZY) {
            lo = CpuReadFromBus(cpu, addr); addr++;
            hi = 0x00;
            cstr_append_fmt(&sInst, "($%02X), Y {IZY}", lo);
        } else if (cpu->lookup[opcode].addrmode == ABS) {
            lo = CpuReadFromBus(cpu, addr); addr++;
            hi = CpuReadFromBus(cpu, addr); addr++;
            cstr_append_fmt(&sInst, "$%04X {ABS}", (uint16_t)(hi << 8) | lo);
        } else if (cpu->lookup[opcode].addrmode == ABX) {
            lo = CpuReadFromBus(cpu, addr); addr++;
            hi = CpuReadFromBus(cpu, addr); addr++;
            cstr_append_fmt(&sInst, "$%04X, X {ABX}", (uint16_t)(hi << 8) | lo);
        } else if (cpu->lookup[opcode].addrmode == ABY) {
            lo = CpuReadFromBus(cpu, addr); addr++;
            hi = CpuReadFromBus(cpu, addr); addr++;
            cstr_append_fmt(&sInst, "$%04X, Y {ABY}", (uint16_t)(hi << 8) | lo);
        } else if (cpu->lookup[opcode].addrmode == IND) {
            lo = CpuReadFromBus(cpu, addr); addr++;
            hi = CpuReadFromBus(cpu, addr); addr++;
            cstr_append_fmt(&sInst, "($%04X) {IND}", (uint16_t)(hi << 8) | lo);
        } else if (cpu->lookup[opcode].addrmode == REL) {
            value = CpuReadFromBus(cpu, addr); addr++;
            cstr_append_fmt(&sInst, "$%02X [$%04X] {REL}", value, addr + (int8_t)value);
        }

        hmap_uint16_str_insert(&mapLines, line_addr, sInst);
    }
    return mapLines;
}
