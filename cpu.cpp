#include<iostream>
#include<fstream>
#include<ostream>
#include<string>
#include<time.h>
#include"registers.h"
#include"instructions.h"
#include"cpu.h"

Registers r = Registers();
OpCodeParser opcp = OpCodeParser();

uint8_t *rom;
bool running = true;

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

void clock_loop(){
  struct timespec req;
  req.tv_sec = 0;
  req.tv_nsec = NS_PER_TICK;
  while(running){
    nanosleep(&req, NULL);
  }
}

void fetch_instruction(){
  if(r.pc == 0xcb){
    //oc = cbprefixed[++r.pc];
  } else {
    //oc = cbprefixed[r.pc]; 
  }
}

int main(int argc, char **argv) {
    std::string rom_file = "";   
    if (argc == 2){
      rom_file = argv[1];
    } else {
      std::cout << "Usage: ./gb <rom_file>" << std::endl;
      return 1;
    }
    std::ifstream is (rom_file, std::ifstream::binary);
    //get length of ROM
    is.seekg(0, is.end);
    int length = is.tellg();
    is.seekg(0, is.beg);

    //read into buffer
    rom = new uint8_t [length];
    std::cout << "Reading " << length << " bytes\n";
    is.read( (char*)(&rom[0]), length);
    if (is)
      std::cout << "Read ROM\n";
    else
      std::cout << "Error: only " << is.gcount() << " could be read";
    is.close();
    for(int i=0;i<100;i++){
      fetch_instruction();
    }

    delete[] rom;
    return 0;
}

