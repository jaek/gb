#include<iostream>
#include<fstream>
#include<ostream>
#include<string>
#include<string.h>
#include<cstring>
#include<time.h>
#include<assert.h>
#include"registers.h"
#include"cpu.h"

bool Operand::is16bit(){
  return datatype >= (OpDataType) n16;
}

CPU::CPU(uint8_t *buffer){
  rom = buffer;
  running = true;
  unprefixed = new OpCode [OC_MAX_OPCODE];
  cbprefixed = new OpCode [OC_MAX_OPCODE];
  init_opcodes();
   
}

void CPU::op_nop(){
  return;
}

void CPU::clock_loop(){
  struct timespec req;
  req.tv_sec = 0;
  req.tv_nsec = NS_PER_TICK;

  OpCode curr;
  Operand op1;
  Operand op2;
  int debug_n = 1000; //n_instructions to execute
  int i = 0;
  while(running && debug_n){
    nanosleep(&req, NULL);
    if(i == 0){
      curr = fetch_instruction();
      get_operands(curr, op1, op2);
      //run command - return clock cycles
    } else {
      i--;
      debug_n--;
    }
  }
}

uint8_t CPU::get_rom_next8(int offset){
  return rom[r.pc+offset];
}

uint16_t CPU::get_rom_next16(int offset){
  uint8_t h = get_rom_next8(offset);
  uint8_t l = get_rom_next8(++offset);
  return (((uint16_t) h << 8 ) & 0xff00) | (l & 0xff);
}

uint16_t CPU::get_operand16(OpDataType dt, char * name, int offset){
  switch(dt){
    case a16:
      return get_rom_next16(offset);
    case n16:
      return get_rom_next16(offset);
    case reg16:
      switch(name[0]){
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

uint8_t CPU::get_operand8(OpDataType dt, char * name, int offset){
  switch(dt){
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
      
void CPU::get_operands(OpCode oc, Operand &op1, Operand &op2){
  int offset = 1;
  op1.datatype = get_operand_type(oc.op1_name);
  if (op1.datatype == none) { return; }
  if (op1.is16bit()){
    op1.data16 = get_operand16(op1.datatype, oc.op1_name, offset);
    offset += 2;
  } else {
    op1.data8 = get_operand8(op1.datatype, oc.op1_name, offset);
    offset += 1;
  }
  op2.datatype = get_operand_type(oc.op2_name);
  if (op2.datatype == none) { return; }
  if (op2.is16bit()){
    op2.data16 = get_operand16(op2.datatype, oc.op2_name, offset);
  } else {
    op2.data8 = get_operand8(op2.datatype, oc.op2_name, offset);
  }
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

void CPU::op_add8(OpCode oc){
  OpCode oc2 = oc;
  oc = oc2;
  oc2 = oc;
  return;
}

//int cpu_tests(uint8_t *rom){
//  //CPU cpu = CPU(rom);
//  return 0;
//}

//int main(int argc, char **argv) {
//
//    uint8_t *rom;
//
//    std::string rom_file = "";   
//    if (argc == 2){
//      rom_file = argv[1];
//    } else {
//      std::cout << "Usage: ./gb <rom_file>" << std::endl;
//      return 1;
//    }
//    std::ifstream is (rom_file, std::ifstream::binary);
//    //get length of ROM
//    is.seekg(0, is.end);
//    int length = is.tellg();
//    is.seekg(0, is.beg);
//
//    //read into buffer
//    rom = new uint8_t [length];
//    std::cout << "Reading " << length << " bytes\n";
//    is.read( (char*)(&rom[0]), length);
//    if (is)
//      std::cout << "Read ROM\n";
//    else
//      std::cout << "Error: only " << is.gcount() << " could be read";
//    is.close();
//
//    CPU cpu = CPU(rom);
//    cpu.r.print_registers();
//    return 0;
//}

