#include <iostream>
#include <fstream>
#include <assert.h>

#include "instructions.h"
#include "registers.h"
#include "cpu.h"

size_t max_test_rom = 100;

uint8_t test_rom1 [] = {
    0x16, 0x01, // LD D, 0x01
    0xFF, 0x00
};

uint8_t test_rom2 [] = {
    0xC6, 0x01, // ADD A, n8
    0xC6, 0x01, // ADD A, n8
    0xC6, 0x01, // ADD A, n8
    0xC6, 0x01, // ADD A, n8
    0xC6, 0x01, // ADD A, n8
    0xC6, 0x01, // ADD A, n8
    0xC6, 0x01, // ADD A, n8
    0xC6, 0x01, // ADD A, n8
    0xC6, 0x01, // ADD A, n8
    0xC6, 0x01, // ADD A, n8
    0xC6, 0x01, // ADD A, n8
    0xC6, 0x01, // ADD A, n8
    0xC6, 0x01, // ADD A, n8
    0xC6, 0x01, // ADD A, n8
    0xC6, 0x01, // ADD A, n8
    0xC6, 0x01, // ADD A, n8
    0xC6, 0x01, // ADD A, n8
    0xC6, 0x01 // ADD A, n8
};

void cpu_test1(CPU &c){
    std::cout << "starting test 2" << std::endl;
//    Operand op1;
    //Operand op2;


    //c.r.pc = 0;

    //assert(c.get_rom_next8(0) == 0x16);
    //assert(c.get_rom_next16(0) == 0x1601);

    //assert(c.get_operand8(rom, n8, 0) == test_rom1[0]);
    //assert(c.get_operand8(rom, n8, 1) == test_rom1[1]);
    //assert(c.get_operand8(rom, n8, 2) == test_rom1[2]);

    //c.r.d = 0xf2;
    //assert(c.get_operand8(D, reg8, 0) == 0xf2);

    //assert(c.get_operand16(rom, n16, 1) == 0x01ff);

    //c.r.set_af(0x1234);
    //assert(c.get_operand16(AF, reg16, 0) == 0x1234);

    //assert(op1.is16bit() == false);
    //op1.datatype = n16;
    //assert(op1.is16bit() == true);

    //// {0x16, "LD", 2, '-', '-', '-', '-', 8, 0, reg8, D, n8, rom};
    //c.fetch_operands(get_op(0x16), op1, op2);
    //assert(op1.datatype == reg8);
    //assert(op1.data8 = c.r.get_register8(D));
    //assert(op2.datatype == n8);
    //assert(op2.data8 = c.get_rom_next8(1));

    //// {0x12, "LD", 1, '-', '-', '-', '-', 8, 0, reg16, DE, reg8, A};
    //c.fetch_operands(get_op(0x12), op1, op2);
    //assert(op1.datatype == reg16);
    //assert(op1.data8 = c.r.get_register8(AF));
    //assert(op2.datatype == reg8);
    //assert(op2.data8 = c.r.get_register8(A));
}

void cpu_test2(CPU &c){
    std::cout << "starting test 2" << std::endl;
    c.ROM = test_rom2;
    c.r.init_values();
    c.r.pc = 0x00;
    c.clock_loop();
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
  assert(r.get_register8(F) == 0xf0);
}

void run_tests(){
    CPU cpu(test_rom1);
    register_tests(cpu.r);
    cpu_test1(cpu);
    cpu_test2(cpu);
}

int main() {
    run_tests();
    return 0;
}