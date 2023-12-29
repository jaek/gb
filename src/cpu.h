#ifndef CPU_H
#define CPU_H

#include "registers.h"

#define CLOCK_SPEED_HZ 1000000
#define SECOND_NS 1000000000
#define NS_PER_TICK (SECOND_NS/CLOCK_SPEED_HZ)
#define MAX_ROM_SIZE 1000000 // 8 Mbit = 1000000 bytes

#define OC_MAX_OPCODE 0xff
#define OC_CB_PREFIX 0xcb

class CPU{

    public:
    bool running;

    uint8_t *ROM;

    uint8_t get_rom_next8(int);
    uint16_t get_rom_next16(int);

    Registers r;

    void clock_loop();
    void show_state();

    OpCode fetch_instruction();
    bool is_cb();
    uint8_t execute_instruction(OpCode, uint16_t, uint16_t);

    uint16_t get_operand_data(OpId, OpDataType, uint8_t);

    void halt();
    uint8_t op_nop(OpCode, uint16_t, uint16_t);
    uint8_t op_add8(OpCode, uint16_t, uint16_t);
    CPU(uint8_t*);

};

#endif