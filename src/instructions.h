#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <cstdint>

enum OpDataType
{
  none=0x0000,
  bit=0x0100,
  reg8=0x0101,
  flag=0x0102,
  vector=0x0103,
  n8=0x0104,
  a8=0x0105,
  s8=0x0106,
  reg16=0x0207,
  n16=0x0208,
  a16=0x0209
};

uint8_t opDataLen(OpDataType);

enum OpId { 
  b0, b1, b2, b3, b4, b5, b6, b7,
  A, B, C, D, E, F, H, L,
  AF, BC, DE, HL, SP, 
  NC, NZ, Z, CF,
  v00, v08, v10, v18, v20, v28, v30, v38,
  rom
};  

struct Operand
{
  OpDataType datatype;
  OpId id;
  union
  {
    uint8_t data8;
    uint16_t data16;
  };
  bool is16bit();
  uint8_t get_offset();
  void show();
  Operand();
};

struct OpCode
{
  uint8_t opcode;
  char mnemonic[11];
  uint8_t bytes;
  char z;
  char n;
  char h;
  char c;
  uint8_t cycles_success;
  uint8_t cycles_failed;
  OpDataType op1_t;
  OpId op1_id;
  OpDataType op2_t;
  OpId op2_id;
};

OpCode get_op(uint8_t);
OpCode get_cb(uint8_t);

OpDataType get_operand_type(char *);

#endif