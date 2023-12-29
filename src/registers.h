#ifndef REGISTERS_H
#define REGISTERS_H

#include <cstdint>
#include "instructions.h"

typedef uint8_t r8;
typedef uint16_t r16;
typedef int8_t e8; //some values are signed

struct Registers
{
  // 8 * 8 bit registers
  r8 a;
  r8 b;
  r8 c;
  r8 d;
  r8 e;
  r8 f; //flags
  r8 h;
  r8 l;

  // 2 * 16 bit registers
  r16 sp;
  r16 pc;

  uint8_t get_mask(char);
  bool get_flag(char);
  void set_flag(char);
  void unset_flag(char);

  r8 get_register8(OpId);
  r16 get_register16(OpId);
  uint16_t get_af();
  void set_af(uint16_t);
  uint16_t get_bc();
  void set_bc(uint16_t);
  uint16_t get_de();
  void set_de(uint16_t);
  uint16_t get_hl();
  void set_hl(uint16_t);
  void init_values();
  void print_registers();
  Registers();
};

#endif