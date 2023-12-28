#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <cstdint>

enum OpDataType { none, n8, a8, s8, reg8, bit, zero, vector, n16, a16, reg16};

struct Operand {
  OpDataType datatype;
  union {
    uint8_t  data8;
    uint16_t data16;
  };
  bool is16bit();
  uint8_t get_offset();
  void show();
  Operand();
};

struct OpCode {
  uint8_t opcode;
  char mnemonic[11];
  uint8_t bytes;
  char z;
  char n;
  char h;
  char c;
  uint8_t cycles_success;
  uint8_t cycles_failed;
  char op1_name[4];
  uint8_t op1_bytes;
  char op2_name[4];
  uint8_t op2_bytes;
};

OpCode get_op(uint8_t);
OpCode get_cb_op(uint8_t);

OpDataType get_operand_type(char*);

#endif