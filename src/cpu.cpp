#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <string.h>
#include <cstring>
#include <time.h>
#include <assert.h>
#include "registers.h"
#include "instructions.h"
#include "cpu.h"

CPU::CPU(uint8_t *buffer)
{
  rom = buffer;
  r = Registers();
  running = true;
}

void CPU::halt()
{
  running = false;
}

void CPU::show_state()
{
  r.print_registers();
}

OpCode CPU::fetch_instruction()
{
  uint8_t opcode = get_rom_next8(0);
  // todo: check !pc>rom_len
  if (opcode == OC_CB_PREFIX)
  {
    return get_cb_op(get_rom_next8(1));
  }
  return get_op(opcode);
}

uint8_t CPU::fetch_operands(OpCode oc, Operand &op1, Operand &op2)
{
  uint8_t offset = 1;
  op1.datatype = get_operand_type(oc.op1_name);
  op2.datatype = get_operand_type(oc.op2_name);

  if (op1.datatype == none)
  {
    return 0;
  }
  if (op1.is16bit())
  {
    op1.data16 = get_operand16(op1.datatype, oc.op1_name, offset);
  }
  else
  {
    op1.data8 = get_operand8(op1.datatype, oc.op1_name, offset);
  }

  if (op2.datatype == none)
  {
    return op1.get_offset();
  }

  offset += op1.get_offset();

  if (op2.is16bit())
  {
    op2.data16 = get_operand16(op2.datatype, oc.op2_name, offset);
  }
  else
  {
    op2.data8 = get_operand8(op2.datatype, oc.op2_name, offset);
  }

  return op1.get_offset() + op2.get_offset();
}

uint8_t CPU::execute_instruction(OpCode oc, Operand op1, Operand op2)
{
  if (!strcmp(oc.mnemonic, "ADD"))
  {
    return op_add8(oc, op1, op2);
  }
  else
  {
    // dump state
    halt();
  }
  return 0;
}

void CPU::clock_loop()
{
  struct timespec req;

  req.tv_sec = 0;
  req.tv_nsec = NS_PER_TICK;
#ifdef unit_test
  req.tv_nsec = 100000000L;
#endif

  OpCode curr;
  Operand op1;
  Operand op2;
  uint8_t i = 0;
  while (running)
  {
    nanosleep(&req, NULL);
    if (i == 0)
    {
      curr = fetch_instruction();
      fetch_operands(curr, op1, op2);
      // run command - return clock cycles
      i = execute_instruction(curr, op1, op2);

#ifdef unit_test
      std::cout << "Instruction: " << curr.mnemonic
                << " " << curr.op1_name
                << " " << curr.op2_name
                << " ( ";
      op1.show();
      std::cout << " ";
      op2.show();
      std::cout << " )" << std::endl;
      show_state();
#endif
    }
    else
    {
      i--;
    }
  }
}

uint8_t CPU::get_rom_next8(int offset)
{
  return rom[r.pc + offset];
}

uint16_t CPU::get_rom_next16(int offset)
{
  uint8_t h = get_rom_next8(offset);
  uint8_t l = get_rom_next8(++offset);
  return (((uint16_t)h << 8) & 0xff00) | (l & 0xff);
}


uint8_t CPU::get_operand8(OpDataType dt, char *name, int offset)
{
  switch (dt)
  {
  case n8:
    return get_rom_next8(offset);
  case a8:
    return get_rom_next8(offset);
  case s8:
    return get_rom_next8(offset);
  case reg8:
    return r.get_register_by_name(name[0]);
  case bit:
    return name[0] - '0';
  case vector:
    break;
  case zero:
    break;
  default:
    break;
  }
  return 0;
}

uint16_t CPU::get_operand16(OpDataType dt, char *name, int offset)
{
  switch (dt)
  {
  case a16:
    return get_rom_next16(offset);
  case n16:
    return get_rom_next16(offset);
  case reg16:
    switch (name[0])
    {
    case 'A':
      return r.get_af();
    case 'B':
      return r.get_bc();
    case 'D':
      return r.get_de();
    case 'H':
      return r.get_hl();
    default:
      return r.sp;
    }
    break;
  default:
    break;
  }
  return 0;
}


// ADD | 0x09 : HL, BC
// ADD | 0x19 : HL, DE
// ADD | 0x29 : HL, HL
// ADD | 0x39 : HL, SP

// ADD | 0x80 : A, B
// ADD | 0x81 : A, C
// ADD | 0x82 : A, D
// ADD | 0x83 : A, E
// ADD | 0x84 : A, H
// ADD | 0x85 : A, L
// ADD | 0x86 : A, HL
// ADD | 0x87 : A, A
// ADD | 0xC6 : A, n8

uint8_t CPU::op_add8(OpCode oc, Operand op1, Operand op2)
{
  if ((int)op1.data8 + op2.data8 > 0xff)
  {
    // handle_overflow
  }
  else
  {
    r.a += op2.data8;
  }
  return oc.cycles_success;
}

uint8_t CPU::op_nop(OpCode oc, Operand op1, Operand op2)
{
  (void)op1;
  (void)op2;
  return oc.cycles_success;
}



#ifndef unit_test
int main(int argc, char **argv)
{

  uint8_t *rom;

  std::string rom_file = "";
  if (argc == 2)
  {
    rom_file = argv[1];
  }
  else
  {
    std::cout << "Usage: ./gb <rom_file>" << std::endl;
    return 1;
  }
  std::ifstream is(rom_file, std::ifstream::binary);
  // get length of ROM
  is.seekg(0, is.end);
  int length = is.tellg();
  is.seekg(0, is.beg);

  // read into buffer
  rom = new uint8_t[length];
  std::cout << "Reading " << length << " bytes\n";
  is.read((char *)(&rom[0]), length);
  if (is)
    std::cout << "Read ROM\n";
  else
    std::cout << "Error: only " << is.gcount() << " could be read";
  is.close();

  CPU cpu = CPU(rom);
  cpu.r.print_registers();
  return 0;
}
#endif