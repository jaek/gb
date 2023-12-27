#include <iostream>
#include <fstream>
#include <assert.h>

#include "registers.h"
#include "cpu.h"

uint8_t test_rom1 [] = {
    0x16, 0x01, // LD D, 0x01
    0xFF
};

void cpu_test(CPU &c){

    c.rom = test_rom1;
    c.r.pc = 0;

    assert(c.get_rom_next8(0) == 0x16);
    assert(c.get_rom_next16(0) == 0x1601);


    char id [4] = "AF";

    assert(c.get_operand8(n8, id, 0) == test_rom1[0]);
    assert(c.get_operand8(n8, id, 1) == test_rom1[1]);
    assert(c.get_operand8(n8, id, 2) == test_rom1[2]);

    assert(c.get_operand16(n16, id, 1) == 0x01ff);

    c.r.set_af(0x1234);
    assert(c.get_operand16(reg16, id, 0) == 0x1234);
    assert(c.get_operand16(reg16, id, 0) == 0x1234);

    Operand op1;
    Operand op2;
    OpCode oc = c.unprefixed[0x16]; //ld d, u8
    c.get_operands(c.unprefixed[0x16], op1, op2);
    assert(op1.datatype == n8);
    assert(op1.datatype == none);
    assert(op1.data8 == 0x01);

}

void register_tests(Registers &r){
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

void run_tests(CPU c){
    register_tests(c.r);
    cpu_test(c);
}

int main(int argc, char **argv) {

    uint8_t *rom;
    rom = test_rom1;
    CPU cpu = CPU(rom);
    cpu.r.print_registers();
    run_tests(cpu);
    return 0;
}