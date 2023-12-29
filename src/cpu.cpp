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
  ROM = buffer;
  r = Registers();
  running = true;
}

bool CPU::is_cb(){
  return (ROM[r.pc] == 0xcb);
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
  if (opcode == OC_CB_PREFIX)
  {
    return get_cb(get_rom_next8(1));
  }
  return get_op(opcode);
}

uint8_t CPU::execute_instruction(OpCode oc, uint16_t op1, uint16_t op2)
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

  int offset;

  uint16_t op1;
  uint16_t op2;

  uint8_t i = 0;
  while (running)
  {
    nanosleep(&req, NULL);
    if (i == 0) 
    {
      offset = 1;

      op1 = 0;
      op2 = 0;

      curr = fetch_instruction();
#ifdef unit_test
      std::cout << "Fetched instruction: 0x" << std::hex << (int) curr.opcode << std::endl;
      std::cout << "Name: " << curr.mnemonic << std::endl;
      std::cout << "Type 1: " << curr.op1_t << " Type 2: " << curr.op2_t << std::endl;
#endif

      offset = offset + is_cb();
      op1 = get_operand_data(curr.op1_id, curr.op1_t, offset);
      op2 = get_operand_data(curr.op2_id, curr.op2_t, offset+opDataLen(curr.op1_t));
      //  case 0xc6:
      // return {0xc6, "ADD", 2, 'Z', '0', 'H', 'C', 8, 0, reg8, A, n8, rom};
      //op1 = (opDataLen(curr.op1_t) != 2) ? (op1 & 0x00ff) : op1;
      //op2 = (opDataLen(curr.op2_t) != 2) ? (op2 & 0x00ff) : op2;

#ifdef unit_test
      std::cout << "1: " << std::hex << (int) ROM[offset] << " Offset: " << offset << std::endl;
      std::cout << "2: " << std::hex << (int) ROM[offset+1] << " Offset: " << offset+1 << std::endl;
      std::cout << "3: " << std::hex << (int) ROM[offset+2] << " Offset: " << offset+opDataLen(curr.op1_t) << std::endl;
      std::cout << "4: " << std::hex << (int) ROM[offset+3] << " Offset: " << std::endl;
      std::cout << "Operand 1: 0x" << std::hex << (int) op1 << std::endl;
      std::cout << "Operand 2: 0x" << std::hex << (int) op1 << std::endl;
      std::cout << "Cycles Left: " << i << std::endl;
      //show_state();
#endif
      execute_instruction(curr, op1, op2);
    }
    else
    {
      std::cout << "Cycles Left: " << i << std::endl;
      i--;
    }
  }
}

uint8_t CPU::get_rom_next8(int offset)
{
  return ROM[r.pc + offset];
}

uint16_t CPU::get_rom_next16(int offset)
{
  uint8_t h = get_rom_next8(offset);
  uint8_t l = get_rom_next8(offset+1);
  return (((uint16_t)h << 8) & 0xff00) | (l & 0xff);
}

uint16_t CPU::get_operand_data(OpId id, OpDataType t, uint8_t offset)
{
  switch (t)
  {
  case a16:
    return get_rom_next16(offset);
  case n16:
    return get_rom_next16(offset);
  case reg16:
    return r.get_register16(id);
  case n8:
    return get_rom_next8(offset);
  case a8:
    return get_rom_next8(offset);
  case s8:
    return get_rom_next8(offset);
  case reg8:
    return r.get_register8(id);
  case bit:
    return (uint8_t) id;
  case vector:
  case flag:
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

uint8_t CPU::op_add8(OpCode oc, uint16_t op1, uint16_t op2)
{
  if (op1 + op2 > 0xff)
  {
    // handle_overflow
  }
  else
  {
    r.a += (op2 & 0x00ff);
  }
  return oc.cycles_success;
}

uint8_t CPU::op_nop(OpCode oc, uint16_t op1, uint16_t op2)
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
