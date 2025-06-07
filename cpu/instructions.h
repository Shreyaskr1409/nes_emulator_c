#pragma once
#include "cpu.h"
#include "addressing_modes.h"
#include <stdint.h>

uint8_t ADC(cpu6502 *cpu); //
uint8_t AND(cpu6502 *cpu); //
uint8_t ASL(cpu6502 *cpu); //
uint8_t BCC(cpu6502 *cpu); //
uint8_t BCS(cpu6502 *cpu); //
uint8_t BEQ(cpu6502 *cpu); //
uint8_t BIT(cpu6502 *cpu); //
uint8_t BMI(cpu6502 *cpu); //
uint8_t BNE(cpu6502 *cpu); //
uint8_t BPL(cpu6502 *cpu); //
uint8_t BRK(cpu6502 *cpu); //
uint8_t BVC(cpu6502 *cpu); //
uint8_t BVS(cpu6502 *cpu); //
uint8_t CLC(cpu6502 *cpu); //
uint8_t CLD(cpu6502 *cpu); //
uint8_t CLI(cpu6502 *cpu); //
uint8_t CLV(cpu6502 *cpu); //
uint8_t CMP(cpu6502 *cpu); //
uint8_t CPX(cpu6502 *cpu); //
uint8_t CPY(cpu6502 *cpu); //
uint8_t DEC(cpu6502 *cpu); //
uint8_t DEX(cpu6502 *cpu); //
uint8_t DEY(cpu6502 *cpu); //
uint8_t EOR(cpu6502 *cpu); //
uint8_t INC(cpu6502 *cpu); //
uint8_t INX(cpu6502 *cpu); //
uint8_t INY(cpu6502 *cpu); //
uint8_t JMP(cpu6502 *cpu); //
uint8_t JSR(cpu6502 *cpu); //
uint8_t LDA(cpu6502 *cpu); //
uint8_t LDX(cpu6502 *cpu); //
uint8_t LDY(cpu6502 *cpu); //
uint8_t LSR(cpu6502 *cpu);
uint8_t NOP(cpu6502 *cpu);
uint8_t ORA(cpu6502 *cpu);
uint8_t PHA(cpu6502 *cpu);
uint8_t PHP(cpu6502 *cpu); //
uint8_t PLA(cpu6502 *cpu); //
uint8_t PLP(cpu6502 *cpu);
uint8_t ROL(cpu6502 *cpu);
uint8_t ROR(cpu6502 *cpu);
uint8_t RTI(cpu6502 *cpu);
uint8_t RTS(cpu6502 *cpu);
uint8_t SBC(cpu6502 *cpu); //
uint8_t SEC(cpu6502 *cpu); //
uint8_t SED(cpu6502 *cpu); //
uint8_t SEI(cpu6502 *cpu); //
uint8_t STA(cpu6502 *cpu); //
uint8_t STX(cpu6502 *cpu); //
uint8_t STY(cpu6502 *cpu); //
uint8_t TAX(cpu6502 *cpu); //
uint8_t TAY(cpu6502 *cpu); //
uint8_t TSX(cpu6502 *cpu); //
uint8_t TXA(cpu6502 *cpu); //
uint8_t TXS(cpu6502 *cpu); //
uint8_t TYA(cpu6502 *cpu); //

uint8_t XXX(cpu6502 *cpu);
