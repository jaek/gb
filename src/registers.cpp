#include<cstdint>
#include<assert.h> 
#include<iostream>
#include"registers.h"

struct HexCharStruct{
  uint8_t c;
  HexCharStruct(uint8_t _c) : c(_c) { }
};

inline std::ostream& operator<<(std::ostream& o, const HexCharStruct& hs){
  return (o << std::hex << (int)hs.c);
}

inline HexCharStruct hex(uint8_t _c){
  return HexCharStruct(_c);
}

Registers::Registers(){
  init_values();
}

uint8_t Registers::get_mask(char flag){
  switch(flag){
    case 'z':
      return 0x80;
    case 'n': 
      return 0x40;
    case 'h':
      return 0x20;
    case 'c': 
      return 0x10;
  }
  return 0x00;

}
bool Registers::get_flag(char flag){
  return (f & get_mask(flag)) > 0;
}

void Registers::set_flag(char flag){ f = f | get_mask(flag) ;}

void Registers::unset_flag(char flag){ f = get_mask(flag) ? !f : f ;}
// There are four virtual 16 bit registers: af bc de hl
// af
uint16_t Registers::get_af(){
  uint16_t v = a;
  return (v << 8) | f;
} 
void Registers::set_af(uint16_t v){
  a = (v & 0xFF00) >> 8;
  f = (v & 0xFF);
}
// bc
uint16_t Registers::get_bc(){
  uint16_t v = b;
  return (v << 8) | c;
} 
void Registers::set_bc(uint16_t v){
  b = (v & 0xFF00) >> 8;
  c = (v & 0xFF);
}
//de
uint16_t Registers::get_de(){
  uint16_t v = d;
  return (v << 8) | e;
} 
void Registers::set_de(uint16_t v){
  d = (v & 0xFF00) >> 8;
  e = (v & 0xFF);
}
//hl
uint16_t Registers::get_hl(){
  uint16_t v = h;
  return (v << 8) | l;
} 

void Registers::set_hl(uint16_t v){
  h = (v & 0xFF00) >> 8;
  l = (v & 0xFF);
}

void Registers::init_values(){
  //DMG-01
  a = 0x01;
  b = 0xFF;
  c = 0x13;
  d = 0x00;
  e = 0xc1;
  f = 0x00;
  h = 0x84;
  l = 0x03;
  pc = 0x0100;
  sp = 0xFFFE;
}

void Registers::print_registers(){
  std::cout << "Register Contents" << std::endl;
  std::cout << "A  = 0x" << hex(a) << std::endl;
  std::cout << "B  = 0x" << hex(b) << std::endl;
  std::cout << "C  = 0x" << hex(c) << std::endl;
  std::cout << "D  = 0x" << hex(d) << std::endl;
  std::cout << "E  = 0x" << hex(e) << std::endl;
  std::cout << "F  = 0x" << hex(f) << std::endl;
  std::cout << "H  = 0x" << hex(h) << std::endl;
  std::cout << "L  = 0x" << hex(l) << std::endl;
  std::cout << "SP = 0x" << hex(sp) << std::endl;
  std::cout << "PC = 0x" << hex(pc) << std::endl;
}

r8 Registers::get_register_by_name(char r){
  switch(r){
    case 'A':
      return this->a;
    case 'B':
      return this->b;
    case 'C':
      return this->c;
    case 'D':
      return this->d;
    case 'E':
      return this->e;
    case 'F':
      return this->f;
    case 'H':
      return this->f;
    case 'L':
      return this->l;
    default:
      return 1;
  }
}

