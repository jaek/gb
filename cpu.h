#ifndef CPU_H
#define CPU_H

#define CLOCK_SPEED_HZ 1000000
#define SECOND_NS 1000000000
#define NS_PER_TICK (SECOND_NS/CLOCK_SPEED_HZ)
#define MAX_ROM_SIZE 1000000 // 8 Mbit = 1000000 bytes

void clock_loop();

#endif