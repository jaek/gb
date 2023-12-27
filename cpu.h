#ifndef CPU_H
#define CPU_H

#include "registers.h"

#define CLOCK_SPEED_HZ 1000000
#define SECOND_NS 1000000000
#define NS_PER_TICK (SECOND_NS/CLOCK_SPEED_HZ)
#define MAX_ROM_SIZE 1000000 // 8 Mbit = 1000000 bytes

#define OC_MAX_OPCODE 0xfa
#define OC_CB_PREFIX 0xcb

enum OpDataType { none, n8, a8, s8, reg8, bit, zero, vector, n16, a16, reg16};

struct Operand {
  OpDataType datatype;
  union {
    uint8_t  data8;
    uint16_t data16;
  };
  bool is16bit();
};

struct OpCode {
  bool valid;
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

class CPU{

    public:
    bool running;

    uint8_t *rom;

    uint8_t get_rom_next8(int);
    uint16_t get_rom_next16(int);

    OpCode* unprefixed;
    OpCode* cbprefixed;

    Registers r;

    void init_opcodes();

    void clock_loop();
    OpCode fetch_instruction();
    void get_operands(OpCode oc, Operand &op1, Operand &op2);
    OpDataType get_operand_type(char *s);
    uint8_t get_operand8(OpDataType, char*, int);
    uint16_t get_operand16(OpDataType, char*, int);

    void op_nop();
    void op_add8(OpCode oc);
    CPU(uint8_t *rom);

};

#endif