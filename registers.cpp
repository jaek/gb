#include<cstdint>
#include<assert.h> 

#include"registers.h"

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

void run_tests(){
  Registers r = Registers();
  assert(r.get_flag('c') == 0);
  r.set_flag('c');
  assert(r.get_flag('c') == 1);
  r.unset_flag('c');
  assert(r.get_flag('c') == 0);
  r.unset_flag('c');
  assert(r.get_flag('c') == 0);
  r.set_af(0x0000);
  r.set_flag('c');
  r.set_flag('z');
  r.set_flag('n');
  r.set_flag('h');
  assert(r.get_af() == 0x00f0);
}