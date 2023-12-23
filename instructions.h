#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "registers.h"

#define MAX_OPCODE 0xfa

struct OpCode {
  bool valid;
  char mnemonic[11];
  uint8_t bytes;
  int8_t z;
  int8_t n;
  int8_t h;
  int8_t c;
  uint8_t cycles_success;
  uint8_t cycles_failed;
  char op1_name[4];
  uint8_t op1_bytes;
  char op2_name[4];
  uint8_t op2_bytes;
};

class OpCodeParser{
  OpCode unprefixed[MAX_OPCODE];
  OpCode cbprefixed[MAX_OPCODE];
  public:
  void init_opcodes();
  OpCode * get_opcode(uint8_t);
  OpCodeParser(){ init_opcodes(); };
}; 

r8 add(Registers &r, r8 a, r8 b);
//A,r8 - A,n8 - A,[HL]
void op_add(Registers &r, r8 source);
//HL,SP - HL,r16
void op_add(Registers &r, r16 source);
//SP,e8
void op_add(Registers &r, e8 source);


//void op_ld(Registers &r, )
#endif