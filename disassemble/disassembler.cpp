#include<iostream>
#include<fstream>
#include <queue>
#include <string.h>

//cartridge header constants
#define CH_START          0x0100
#define CH_END            0x014F
#define CH_SIZE           (CH_END - CH_START) + 1
#define CH_TITLE_START    0x0134
#define CH_TITLE_END      0x0142
#define CH_CART_TYPE      0x0147
#define CH_ROM_SIZE       0x0148
#define CH_RAM_SIZE       0x0149
#define CH_CHECKSUM_START 0x0134
#define CH_CHECKSUM_END   0x014C
#define CH_CHECKSUM       0x014D
#define CH_GLOBAL_CHECKSUM_H 0x014E
#define CH_GLOBAL_CHECKSUM_L 0x014F
#define ROM_START            0x0150

#define LINE_LENGTH 50
#define MAX_LEN 1000000 //largest ROM size is 8Mbit

//header https://gbdev.gg8.se/wiki/articles/The_Cartridge_Header
struct CartridgeHeader{
  unsigned char title[16];
  unsigned char rom_size;
  unsigned char ram_size;
  unsigned char cart_type;
  unsigned char header_checksum;
  unsigned char c_header_checksum;
  u_int16_t global_checksum;
  u_int16_t c_global_checksum;

  u_int16_t calc_global_checksum(unsigned char * buffer, int buffer_len){
    // (sum of all ROM contents) - checksum bytes
    int i = 0; 
    u_int16_t total = 0;

    while (i < CH_GLOBAL_CHECKSUM_H){ 
      total += (unsigned char) buffer[i++];
    }
  
    i = CH_GLOBAL_CHECKSUM_L + 1;
    while (i <= buffer_len){ 
      total += (unsigned char) buffer[i++];
    }
    return total;
  }

  unsigned char calc_header_checksum(unsigned char * buffer){
    //compute the header checksum 
    //x=0:FOR i=0134h TO 014Ch:x=x-MEM[i]-1:NEXT
    unsigned char x = 0;
    int i = 0;
    while (i <= CH_CHECKSUM_END) { 
      x = x - buffer[i] - 1;
      i++;
    }
    return x;
  }

  void display_header_data(){
    std::cout << "Cart Header     \n=============================\n";
    std::cout << "ROM Name: " << title << "\n";
    std::cout << "Cartridge Type: " << std::hex << (int) cart_type << "\n";
    std::cout << "ROM Size: " << std::hex << (int) rom_size << "\n";
    std::cout << "RAM Size: " << std::hex << (int) ram_size << "\n";
    std::cout << "Header Checksum: " << std::hex << header_checksum << "\n";
    std::cout << "Computed Checksum: " << std::hex << c_header_checksum << "\n";
    std::cout << "Global Checksum         : " << global_checksum << "\n";
    std::cout << "Computed Global Checksum: " << c_global_checksum<< "\n";
    std::cout << "=============================\nDisassembled ROM\n=============================\n";

  }

  CartridgeHeader(unsigned char * buffer, int buffer_len){
      int i = 0;
      while (i < 16){
        title[i] = buffer[CH_TITLE_START+i];
        i++;
      }
      cart_type = buffer[CH_CART_TYPE];
      rom_size = buffer[CH_ROM_SIZE];
      ram_size = buffer[CH_RAM_SIZE];
      header_checksum = buffer[CH_CHECKSUM];
      c_header_checksum = calc_header_checksum(buffer);
      global_checksum = (buffer[CH_GLOBAL_CHECKSUM_H] << 8) & 0xFF00 | (0xFF & buffer[CH_GLOBAL_CHECKSUM_L]); 
      c_global_checksum = calc_global_checksum(buffer, buffer_len);
  }
};

char output[MAX_LEN][LINE_LENGTH];

struct OpCode {
  bool valid;
  char mnemonic[11];
  int bytes;
  char op1[4];
  char op2[4]; 
};

OpCode *get_opcode_set(bool cb_prefixed){
  if(cb_prefixed == true){
    return cbprefixed;
  }
  return unprefixed;
}

void init_opcodes(){
  //initialize with dummy values
  for(int i=0;i<=0xFA;i++){
    cbprefixed[i]={false,"*",1,"*","*"};
    unprefixed[i]={false,"*",1,"*","*"};
  } 
  cbprefixed[0x00]={true,"RLC",2,"B","*"};cbprefixed[0x01]={true,"RLC",2,"C","*"};cbprefixed[0x02]={true,"RLC",2,"D","*"};cbprefixed[0x03]={true,"RLC",2,"E","*"};cbprefixed[0x04]={true,"RLC",2,"H","*"};cbprefixed[0x05]={true,"RLC",2,"L","*"};cbprefixed[0x06]={true,"RLC",2,"HL","*"};cbprefixed[0x07]={true,"RLC",2,"A","*"};cbprefixed[0x08]={true,"RRC",2,"B","*"};cbprefixed[0x09]={true,"RRC",2,"C","*"};
  cbprefixed[0x0B]={true,"RRC",2,"E","*"};cbprefixed[0x0C]={true,"RRC",2,"H","*"};cbprefixed[0x0D]={true,"RRC",2,"L","*"};cbprefixed[0x0E]={true,"RRC",2,"HL","*"};cbprefixed[0x0F]={true,"RRC",2,"A","*"};cbprefixed[0x10]={true,"RL",2,"B","*"};cbprefixed[0x11]={true,"RL",2,"C","*"};cbprefixed[0x12]={true,"RL",2,"D","*"};cbprefixed[0x13]={true,"RL",2,"E","*"};cbprefixed[0x14]={true,"RL",2,"H","*"};
  cbprefixed[0x16]={true,"RL",2,"HL","*"};cbprefixed[0x17]={true,"RL",2,"A","*"};cbprefixed[0x18]={true,"RR",2,"B","*"};cbprefixed[0x19]={true,"RR",2,"C","*"};cbprefixed[0x1A]={true,"RR",2,"D","*"};cbprefixed[0x1B]={true,"RR",2,"E","*"};cbprefixed[0x1C]={true,"RR",2,"H","*"};cbprefixed[0x1D]={true,"RR",2,"L","*"};cbprefixed[0x1E]={true,"RR",2,"HL","*"};cbprefixed[0x1F]={true,"RR",2,"A","*"};
  cbprefixed[0x21]={true,"SLA",2,"C","*"};cbprefixed[0x22]={true,"SLA",2,"D","*"};cbprefixed[0x23]={true,"SLA",2,"E","*"};cbprefixed[0x24]={true,"SLA",2,"H","*"};cbprefixed[0x25]={true,"SLA",2,"L","*"};cbprefixed[0x26]={true,"SLA",2,"HL","*"};cbprefixed[0x27]={true,"SLA",2,"A","*"};cbprefixed[0x28]={true,"SRA",2,"B","*"};cbprefixed[0x29]={true,"SRA",2,"C","*"};cbprefixed[0x2A]={true,"SRA",2,"D","*"};
  cbprefixed[0x2C]={true,"SRA",2,"H","*"};cbprefixed[0x2D]={true,"SRA",2,"L","*"};cbprefixed[0x2E]={true,"SRA",2,"HL","*"};cbprefixed[0x2F]={true,"SRA",2,"A","*"};cbprefixed[0x30]={true,"SWAP",2,"B","*"};cbprefixed[0x31]={true,"SWAP",2,"C","*"};cbprefixed[0x32]={true,"SWAP",2,"D","*"};cbprefixed[0x33]={true,"SWAP",2,"E","*"};cbprefixed[0x34]={true,"SWAP",2,"H","*"};cbprefixed[0x35]={true,"SWAP",2,"L","*"};
  cbprefixed[0x37]={true,"SWAP",2,"A","*"};cbprefixed[0x38]={true,"SRL",2,"B","*"};cbprefixed[0x39]={true,"SRL",2,"C","*"};cbprefixed[0x3A]={true,"SRL",2,"D","*"};cbprefixed[0x3B]={true,"SRL",2,"E","*"};cbprefixed[0x3C]={true,"SRL",2,"H","*"};cbprefixed[0x3D]={true,"SRL",2,"L","*"};cbprefixed[0x3E]={true,"SRL",2,"HL","*"};cbprefixed[0x3F]={true,"SRL",2,"A","*"};cbprefixed[0x40]={true,"BIT",2,"0","B"};
  cbprefixed[0x42]={true,"BIT",2,"0","D"};cbprefixed[0x43]={true,"BIT",2,"0","E"};cbprefixed[0x44]={true,"BIT",2,"0","H"};cbprefixed[0x45]={true,"BIT",2,"0","L"};cbprefixed[0x46]={true,"BIT",2,"0","HL"};cbprefixed[0x47]={true,"BIT",2,"0","A"};cbprefixed[0x48]={true,"BIT",2,"1","B"};cbprefixed[0x49]={true,"BIT",2,"1","C"};cbprefixed[0x4A]={true,"BIT",2,"1","D"};cbprefixed[0x4B]={true,"BIT",2,"1","E"};
  cbprefixed[0x4D]={true,"BIT",2,"1","L"};cbprefixed[0x4E]={true,"BIT",2,"1","HL"};cbprefixed[0x4F]={true,"BIT",2,"1","A"};cbprefixed[0x50]={true,"BIT",2,"2","B"};cbprefixed[0x51]={true,"BIT",2,"2","C"};cbprefixed[0x52]={true,"BIT",2,"2","D"};cbprefixed[0x53]={true,"BIT",2,"2","E"};cbprefixed[0x54]={true,"BIT",2,"2","H"};cbprefixed[0x55]={true,"BIT",2,"2","L"};cbprefixed[0x56]={true,"BIT",2,"2","HL"};
  cbprefixed[0x58]={true,"BIT",2,"3","B"};cbprefixed[0x59]={true,"BIT",2,"3","C"};cbprefixed[0x5A]={true,"BIT",2,"3","D"};cbprefixed[0x5B]={true,"BIT",2,"3","E"};cbprefixed[0x5C]={true,"BIT",2,"3","H"};cbprefixed[0x5D]={true,"BIT",2,"3","L"};cbprefixed[0x5E]={true,"BIT",2,"3","HL"};cbprefixed[0x5F]={true,"BIT",2,"3","A"};cbprefixed[0x60]={true,"BIT",2,"4","B"};cbprefixed[0x61]={true,"BIT",2,"4","C"};
  cbprefixed[0x63]={true,"BIT",2,"4","E"};cbprefixed[0x64]={true,"BIT",2,"4","H"};cbprefixed[0x65]={true,"BIT",2,"4","L"};cbprefixed[0x66]={true,"BIT",2,"4","HL"};cbprefixed[0x67]={true,"BIT",2,"4","A"};cbprefixed[0x68]={true,"BIT",2,"5","B"};cbprefixed[0x69]={true,"BIT",2,"5","C"};cbprefixed[0x6A]={true,"BIT",2,"5","D"};cbprefixed[0x6B]={true,"BIT",2,"5","E"};cbprefixed[0x6C]={true,"BIT",2,"5","H"};
  cbprefixed[0x6E]={true,"BIT",2,"5","HL"};cbprefixed[0x6F]={true,"BIT",2,"5","A"};cbprefixed[0x70]={true,"BIT",2,"6","B"};cbprefixed[0x71]={true,"BIT",2,"6","C"};cbprefixed[0x72]={true,"BIT",2,"6","D"};cbprefixed[0x73]={true,"BIT",2,"6","E"};cbprefixed[0x74]={true,"BIT",2,"6","H"};cbprefixed[0x75]={true,"BIT",2,"6","L"};cbprefixed[0x76]={true,"BIT",2,"6","HL"};cbprefixed[0x77]={true,"BIT",2,"6","A"};
  cbprefixed[0x79]={true,"BIT",2,"7","C"};cbprefixed[0x7A]={true,"BIT",2,"7","D"};cbprefixed[0x7B]={true,"BIT",2,"7","E"};cbprefixed[0x7C]={true,"BIT",2,"7","H"};cbprefixed[0x7D]={true,"BIT",2,"7","L"};cbprefixed[0x7E]={true,"BIT",2,"7","HL"};cbprefixed[0x7F]={true,"BIT",2,"7","A"};cbprefixed[0x80]={true,"RES",2,"0","B"};cbprefixed[0x81]={true,"RES",2,"0","C"};cbprefixed[0x82]={true,"RES",2,"0","D"};
  cbprefixed[0x84]={true,"RES",2,"0","H"};cbprefixed[0x85]={true,"RES",2,"0","L"};cbprefixed[0x86]={true,"RES",2,"0","HL"};cbprefixed[0x87]={true,"RES",2,"0","A"};cbprefixed[0x88]={true,"RES",2,"1","B"};cbprefixed[0x89]={true,"RES",2,"1","C"};cbprefixed[0x8A]={true,"RES",2,"1","D"};cbprefixed[0x8B]={true,"RES",2,"1","E"};cbprefixed[0x8C]={true,"RES",2,"1","H"};cbprefixed[0x8D]={true,"RES",2,"1","L"};
  cbprefixed[0x8F]={true,"RES",2,"1","A"};cbprefixed[0x90]={true,"RES",2,"2","B"};cbprefixed[0x91]={true,"RES",2,"2","C"};cbprefixed[0x92]={true,"RES",2,"2","D"};cbprefixed[0x93]={true,"RES",2,"2","E"};cbprefixed[0x94]={true,"RES",2,"2","H"};cbprefixed[0x95]={true,"RES",2,"2","L"};cbprefixed[0x96]={true,"RES",2,"2","HL"};cbprefixed[0x97]={true,"RES",2,"2","A"};cbprefixed[0x98]={true,"RES",2,"3","B"};
  cbprefixed[0x9A]={true,"RES",2,"3","D"};cbprefixed[0x9B]={true,"RES",2,"3","E"};cbprefixed[0x9C]={true,"RES",2,"3","H"};cbprefixed[0x9D]={true,"RES",2,"3","L"};cbprefixed[0x9E]={true,"RES",2,"3","HL"};cbprefixed[0x9F]={true,"RES",2,"3","A"};cbprefixed[0xA0]={true,"RES",2,"4","B"};cbprefixed[0xA1]={true,"RES",2,"4","C"};cbprefixed[0xA2]={true,"RES",2,"4","D"};cbprefixed[0xA3]={true,"RES",2,"4","E"};
  cbprefixed[0xA5]={true,"RES",2,"4","L"};cbprefixed[0xA6]={true,"RES",2,"4","HL"};cbprefixed[0xA7]={true,"RES",2,"4","A"};cbprefixed[0xA8]={true,"RES",2,"5","B"};cbprefixed[0xA9]={true,"RES",2,"5","C"};cbprefixed[0xAA]={true,"RES",2,"5","D"};cbprefixed[0xAB]={true,"RES",2,"5","E"};cbprefixed[0xAC]={true,"RES",2,"5","H"};cbprefixed[0xAD]={true,"RES",2,"5","L"};cbprefixed[0xAE]={true,"RES",2,"5","HL"};
  cbprefixed[0xB0]={true,"RES",2,"6","B"};cbprefixed[0xB1]={true,"RES",2,"6","C"};cbprefixed[0xB2]={true,"RES",2,"6","D"};cbprefixed[0xB3]={true,"RES",2,"6","E"};cbprefixed[0xB4]={true,"RES",2,"6","H"};cbprefixed[0xB5]={true,"RES",2,"6","L"};cbprefixed[0xB6]={true,"RES",2,"6","HL"};cbprefixed[0xB7]={true,"RES",2,"6","A"};cbprefixed[0xB8]={true,"RES",2,"7","B"};cbprefixed[0xB9]={true,"RES",2,"7","C"};
  cbprefixed[0xBB]={true,"RES",2,"7","E"};cbprefixed[0xBC]={true,"RES",2,"7","H"};cbprefixed[0xBD]={true,"RES",2,"7","L"};cbprefixed[0xBE]={true,"RES",2,"7","HL"};cbprefixed[0xBF]={true,"RES",2,"7","A"};cbprefixed[0xC0]={true,"SET",2,"0","B"};cbprefixed[0xC1]={true,"SET",2,"0","C"};cbprefixed[0xC2]={true,"SET",2,"0","D"};cbprefixed[0xC3]={true,"SET",2,"0","E"};cbprefixed[0xC4]={true,"SET",2,"0","H"};
  cbprefixed[0xC6]={true,"SET",2,"0","HL"};cbprefixed[0xC7]={true,"SET",2,"0","A"};cbprefixed[0xC8]={true,"SET",2,"1","B"};cbprefixed[0xC9]={true,"SET",2,"1","C"};cbprefixed[0xCA]={true,"SET",2,"1","D"};cbprefixed[0xCB]={true,"SET",2,"1","E"};cbprefixed[0xCC]={true,"SET",2,"1","H"};cbprefixed[0xCD]={true,"SET",2,"1","L"};cbprefixed[0xCE]={true,"SET",2,"1","HL"};cbprefixed[0xCF]={true,"SET",2,"1","A"};
  cbprefixed[0xD1]={true,"SET",2,"2","C"};cbprefixed[0xD2]={true,"SET",2,"2","D"};cbprefixed[0xD3]={true,"SET",2,"2","E"};cbprefixed[0xD4]={true,"SET",2,"2","H"};cbprefixed[0xD5]={true,"SET",2,"2","L"};cbprefixed[0xD6]={true,"SET",2,"2","HL"};cbprefixed[0xD7]={true,"SET",2,"2","A"};cbprefixed[0xD8]={true,"SET",2,"3","B"};cbprefixed[0xD9]={true,"SET",2,"3","C"};cbprefixed[0xDA]={true,"SET",2,"3","D"};
  cbprefixed[0xDC]={true,"SET",2,"3","H"};cbprefixed[0xDD]={true,"SET",2,"3","L"};cbprefixed[0xDE]={true,"SET",2,"3","HL"};cbprefixed[0xDF]={true,"SET",2,"3","A"};cbprefixed[0xE0]={true,"SET",2,"4","B"};cbprefixed[0xE1]={true,"SET",2,"4","C"};cbprefixed[0xE2]={true,"SET",2,"4","D"};cbprefixed[0xE3]={true,"SET",2,"4","E"};cbprefixed[0xE4]={true,"SET",2,"4","H"};cbprefixed[0xE5]={true,"SET",2,"4","L"};
  cbprefixed[0xE7]={true,"SET",2,"4","A"};cbprefixed[0xE8]={true,"SET",2,"5","B"};cbprefixed[0xE9]={true,"SET",2,"5","C"};cbprefixed[0xEA]={true,"SET",2,"5","D"};cbprefixed[0xEB]={true,"SET",2,"5","E"};cbprefixed[0xEC]={true,"SET",2,"5","H"};cbprefixed[0xED]={true,"SET",2,"5","L"};cbprefixed[0xEE]={true,"SET",2,"5","HL"};cbprefixed[0xEF]={true,"SET",2,"5","A"};cbprefixed[0xF0]={true,"SET",2,"6","B"};
  cbprefixed[0xF2]={true,"SET",2,"6","D"};cbprefixed[0xF3]={true,"SET",2,"6","E"};cbprefixed[0xF4]={true,"SET",2,"6","H"};cbprefixed[0xF5]={true,"SET",2,"6","L"};cbprefixed[0xF6]={true,"SET",2,"6","HL"};cbprefixed[0xF7]={true,"SET",2,"6","A"};cbprefixed[0xF8]={true,"SET",2,"7","B"};cbprefixed[0xF9]={true,"SET",2,"7","C"};cbprefixed[0xFA]={true,"SET",2,"7","D"};cbprefixed[0xFB]={true,"SET",2,"7","E"};
  cbprefixed[0xFD]={true,"SET",2,"7","L"};cbprefixed[0xFE]={true,"SET",2,"7","HL"};cbprefixed[0xFF]={true,"SET",2,"7","A"};unprefixed[0x00]={true,"NOP",1,"*","*"};unprefixed[0x01]={true,"LD",3,"BC","n16"};unprefixed[0x02]={true,"LD",1,"BC","A"};unprefixed[0x03]={true,"INC",1,"BC","*"};unprefixed[0x04]={true,"INC",1,"B","*"};unprefixed[0x05]={true,"DEC",1,"B","*"};unprefixed[0x06]={true,"LD",2,"B","n8"};
  unprefixed[0x08]={true,"LD",3,"a16","SP"};unprefixed[0x09]={true,"ADD",1,"HL","BC"};unprefixed[0x0A]={true,"LD",1,"A","BC"};unprefixed[0x0B]={true,"DEC",1,"BC","*"};unprefixed[0x0C]={true,"INC",1,"C","*"};unprefixed[0x0D]={true,"DEC",1,"C","*"};unprefixed[0x0E]={true,"LD",2,"C","n8"};unprefixed[0x0F]={true,"RRCA",1,"*","*"};unprefixed[0x10]={true,"STOP",2,"n8","*"};unprefixed[0x11]={true,"LD",3,"DE","n16"};
  unprefixed[0x13]={true,"INC",1,"DE","*"};unprefixed[0x14]={true,"INC",1,"D","*"};unprefixed[0x15]={true,"DEC",1,"D","*"};unprefixed[0x16]={true,"LD",2,"D","n8"};unprefixed[0x17]={true,"RLA",1,"*","*"};unprefixed[0x18]={true,"JR",2,"e8","*"};unprefixed[0x19]={true,"ADD",1,"HL","DE"};unprefixed[0x1A]={true,"LD",1,"A","DE"};unprefixed[0x1B]={true,"DEC",1,"DE","*"};unprefixed[0x1C]={true,"INC",1,"E","*"};
  unprefixed[0x1E]={true,"LD",2,"E","n8"};unprefixed[0x1F]={true,"RRA",1,"*","*"};unprefixed[0x20]={true,"JR",2,"NZ","e8"};unprefixed[0x21]={true,"LD",3,"HL","n16"};unprefixed[0x22]={true,"LD",1,"HL","A"};unprefixed[0x23]={true,"INC",1,"HL","*"};unprefixed[0x24]={true,"INC",1,"H","*"};unprefixed[0x25]={true,"DEC",1,"H","*"};unprefixed[0x26]={true,"LD",2,"H","n8"};unprefixed[0x27]={true,"DAA",1,"*","*"};
  unprefixed[0x29]={true,"ADD",1,"HL","HL"};unprefixed[0x2A]={true,"LD",1,"A","HL"};unprefixed[0x2B]={true,"DEC",1,"HL","*"};unprefixed[0x2C]={true,"INC",1,"L","*"};unprefixed[0x2D]={true,"DEC",1,"L","*"};unprefixed[0x2E]={true,"LD",2,"L","n8"};unprefixed[0x2F]={true,"CPL",1,"*","*"};unprefixed[0x30]={true,"JR",2,"NC","e8"};unprefixed[0x31]={true,"LD",3,"SP","n16"};unprefixed[0x32]={true,"LD",1,"HL","A"};
  unprefixed[0x34]={true,"INC",1,"HL","*"};unprefixed[0x35]={true,"DEC",1,"HL","*"};unprefixed[0x36]={true,"LD",2,"HL","n8"};unprefixed[0x37]={true,"SCF",1,"*","*"};unprefixed[0x38]={true,"JR",2,"C","e8"};unprefixed[0x39]={true,"ADD",1,"HL","SP"};unprefixed[0x3A]={true,"LD",1,"A","HL"};unprefixed[0x3B]={true,"DEC",1,"SP","*"};unprefixed[0x3C]={true,"INC",1,"A","*"};unprefixed[0x3D]={true,"DEC",1,"A","*"};
  unprefixed[0x3F]={true,"CCF",1,"*","*"};unprefixed[0x40]={true,"LD",1,"B","B"};unprefixed[0x41]={true,"LD",1,"B","C"};unprefixed[0x42]={true,"LD",1,"B","D"};unprefixed[0x43]={true,"LD",1,"B","E"};unprefixed[0x44]={true,"LD",1,"B","H"};unprefixed[0x45]={true,"LD",1,"B","L"};unprefixed[0x46]={true,"LD",1,"B","HL"};unprefixed[0x47]={true,"LD",1,"B","A"};unprefixed[0x48]={true,"LD",1,"C","B"};
  unprefixed[0x4A]={true,"LD",1,"C","D"};unprefixed[0x4B]={true,"LD",1,"C","E"};unprefixed[0x4C]={true,"LD",1,"C","H"};unprefixed[0x4D]={true,"LD",1,"C","L"};unprefixed[0x4E]={true,"LD",1,"C","HL"};unprefixed[0x4F]={true,"LD",1,"C","A"};unprefixed[0x50]={true,"LD",1,"D","B"};unprefixed[0x51]={true,"LD",1,"D","C"};unprefixed[0x52]={true,"LD",1,"D","D"};unprefixed[0x53]={true,"LD",1,"D","E"};
  unprefixed[0x55]={true,"LD",1,"D","L"};unprefixed[0x56]={true,"LD",1,"D","HL"};unprefixed[0x57]={true,"LD",1,"D","A"};unprefixed[0x58]={true,"LD",1,"E","B"};unprefixed[0x59]={true,"LD",1,"E","C"};unprefixed[0x5A]={true,"LD",1,"E","D"};unprefixed[0x5B]={true,"LD",1,"E","E"};unprefixed[0x5C]={true,"LD",1,"E","H"};unprefixed[0x5D]={true,"LD",1,"E","L"};unprefixed[0x5E]={true,"LD",1,"E","HL"};
  unprefixed[0x60]={true,"LD",1,"H","B"};unprefixed[0x61]={true,"LD",1,"H","C"};unprefixed[0x62]={true,"LD",1,"H","D"};unprefixed[0x63]={true,"LD",1,"H","E"};unprefixed[0x64]={true,"LD",1,"H","H"};unprefixed[0x65]={true,"LD",1,"H","L"};unprefixed[0x66]={true,"LD",1,"H","HL"};unprefixed[0x67]={true,"LD",1,"H","A"};unprefixed[0x68]={true,"LD",1,"L","B"};unprefixed[0x69]={true,"LD",1,"L","C"};
  unprefixed[0x6B]={true,"LD",1,"L","E"};unprefixed[0x6C]={true,"LD",1,"L","H"};unprefixed[0x6D]={true,"LD",1,"L","L"};unprefixed[0x6E]={true,"LD",1,"L","HL"};unprefixed[0x6F]={true,"LD",1,"L","A"};unprefixed[0x70]={true,"LD",1,"HL","B"};unprefixed[0x71]={true,"LD",1,"HL","C"};unprefixed[0x72]={true,"LD",1,"HL","D"};unprefixed[0x73]={true,"LD",1,"HL","E"};unprefixed[0x74]={true,"LD",1,"HL","H"};
  unprefixed[0x76]={true,"HALT",1,"*","*"};unprefixed[0x77]={true,"LD",1,"HL","A"};unprefixed[0x78]={true,"LD",1,"A","B"};unprefixed[0x79]={true,"LD",1,"A","C"};unprefixed[0x7A]={true,"LD",1,"A","D"};unprefixed[0x7B]={true,"LD",1,"A","E"};unprefixed[0x7C]={true,"LD",1,"A","H"};unprefixed[0x7D]={true,"LD",1,"A","L"};unprefixed[0x7E]={true,"LD",1,"A","HL"};unprefixed[0x7F]={true,"LD",1,"A","A"};
  unprefixed[0x81]={true,"ADD",1,"A","C"};unprefixed[0x82]={true,"ADD",1,"A","D"};unprefixed[0x83]={true,"ADD",1,"A","E"};unprefixed[0x84]={true,"ADD",1,"A","H"};unprefixed[0x85]={true,"ADD",1,"A","L"};unprefixed[0x86]={true,"ADD",1,"A","HL"};unprefixed[0x87]={true,"ADD",1,"A","A"};unprefixed[0x88]={true,"ADC",1,"A","B"};unprefixed[0x89]={true,"ADC",1,"A","C"};unprefixed[0x8A]={true,"ADC",1,"A","D"};
  unprefixed[0x8C]={true,"ADC",1,"A","H"};unprefixed[0x8D]={true,"ADC",1,"A","L"};unprefixed[0x8E]={true,"ADC",1,"A","HL"};unprefixed[0x8F]={true,"ADC",1,"A","A"};unprefixed[0x90]={true,"SUB",1,"A","B"};unprefixed[0x91]={true,"SUB",1,"A","C"};unprefixed[0x92]={true,"SUB",1,"A","D"};unprefixed[0x93]={true,"SUB",1,"A","E"};unprefixed[0x94]={true,"SUB",1,"A","H"};unprefixed[0x95]={true,"SUB",1,"A","L"};
  unprefixed[0x97]={true,"SUB",1,"A","A"};unprefixed[0x98]={true,"SBC",1,"A","B"};unprefixed[0x99]={true,"SBC",1,"A","C"};unprefixed[0x9A]={true,"SBC",1,"A","D"};unprefixed[0x9B]={true,"SBC",1,"A","E"};unprefixed[0x9C]={true,"SBC",1,"A","H"};unprefixed[0x9D]={true,"SBC",1,"A","L"};unprefixed[0x9E]={true,"SBC",1,"A","HL"};unprefixed[0x9F]={true,"SBC",1,"A","A"};unprefixed[0xA0]={true,"AND",1,"A","B"};
  unprefixed[0xA2]={true,"AND",1,"A","D"};unprefixed[0xA3]={true,"AND",1,"A","E"};unprefixed[0xA4]={true,"AND",1,"A","H"};unprefixed[0xA5]={true,"AND",1,"A","L"};unprefixed[0xA6]={true,"AND",1,"A","HL"};unprefixed[0xA7]={true,"AND",1,"A","A"};unprefixed[0xA8]={true,"XOR",1,"A","B"};unprefixed[0xA9]={true,"XOR",1,"A","C"};unprefixed[0xAA]={true,"XOR",1,"A","D"};unprefixed[0xAB]={true,"XOR",1,"A","E"};
  unprefixed[0xAD]={true,"XOR",1,"A","L"};unprefixed[0xAE]={true,"XOR",1,"A","HL"};unprefixed[0xAF]={true,"XOR",1,"A","A"};unprefixed[0xB0]={true,"OR",1,"A","B"};unprefixed[0xB1]={true,"OR",1,"A","C"};unprefixed[0xB2]={true,"OR",1,"A","D"};unprefixed[0xB3]={true,"OR",1,"A","E"};unprefixed[0xB4]={true,"OR",1,"A","H"};unprefixed[0xB5]={true,"OR",1,"A","L"};unprefixed[0xB6]={true,"OR",1,"A","HL"};
  unprefixed[0xB8]={true,"CP",1,"A","B"};unprefixed[0xB9]={true,"CP",1,"A","C"};unprefixed[0xBA]={true,"CP",1,"A","D"};unprefixed[0xBB]={true,"CP",1,"A","E"};unprefixed[0xBC]={true,"CP",1,"A","H"};unprefixed[0xBD]={true,"CP",1,"A","L"};unprefixed[0xBE]={true,"CP",1,"A","HL"};unprefixed[0xBF]={true,"CP",1,"A","A"};unprefixed[0xC0]={true,"RET",1,"NZ","*"};unprefixed[0xC1]={true,"POP",1,"BC","*"};
  unprefixed[0xC3]={true,"JP",3,"a16","*"};unprefixed[0xC4]={true,"CALL",3,"NZ","a16"};unprefixed[0xC5]={true,"PUSH",1,"BC","*"};unprefixed[0xC6]={true,"ADD",2,"A","n8"};unprefixed[0xC7]={true,"RST",1,"$00","*"};unprefixed[0xC8]={true,"RET",1,"Z","*"};unprefixed[0xC9]={true,"RET",1,"*","*"};unprefixed[0xCA]={true,"JP",3,"Z","a16"};unprefixed[0xCB]={true,"PREFIX",1,"*","*"};unprefixed[0xCC]={true,"CALL",3,"Z","a16"};
  unprefixed[0xCE]={true,"ADC",2,"A","n8"};unprefixed[0xCF]={true,"RST",1,"$08","*"};unprefixed[0xD0]={true,"RET",1,"NC","*"};unprefixed[0xD1]={true,"POP",1,"DE","*"};unprefixed[0xD2]={true,"JP",3,"NC","a16"};unprefixed[0xD3]={true,"ILLEGAL_D3",1,"*","*"};unprefixed[0xD4]={true,"CALL",3,"NC","a16"};unprefixed[0xD5]={true,"PUSH",1,"DE","*"};unprefixed[0xD6]={true,"SUB",2,"A","n8"};unprefixed[0xD7]={true,"RST",1,"$10","*"};
  unprefixed[0xD9]={true,"RETI",1,"*","*"};unprefixed[0xDA]={true,"JP",3,"C","a16"};unprefixed[0xDB]={true,"ILLEGAL_DB",1,"*","*"};unprefixed[0xDC]={true,"CALL",3,"C","a16"};unprefixed[0xDD]={true,"ILLEGAL_DD",1,"*","*"};unprefixed[0xDE]={true,"SBC",2,"A","n8"};unprefixed[0xDF]={true,"RST",1,"$18","*"};unprefixed[0xE0]={true,"LDH",2,"a8","A"};unprefixed[0xE1]={true,"POP",1,"HL","*"};unprefixed[0xE2]={true,"LD",1,"C","A"};
  unprefixed[0xE4]={true,"ILLEGAL_E4",1,"*","*"};unprefixed[0xE5]={true,"PUSH",1,"HL","*"};unprefixed[0xE6]={true,"AND",2,"A","n8"};unprefixed[0xE7]={true,"RST",1,"$20","*"};unprefixed[0xE8]={true,"ADD",2,"SP","e8"};unprefixed[0xE9]={true,"JP",1,"HL","*"};unprefixed[0xEA]={true,"LD",3,"a16","A"};unprefixed[0xEB]={true,"ILLEGAL_EB",1,"*","*"};unprefixed[0xEC]={true,"ILLEGAL_EC",1,"*","*"};unprefixed[0xED]={true,"ILLEGAL_ED",1,"*","*"};
  unprefixed[0xEF]={true,"RST",1,"$28","*"};unprefixed[0xF0]={true,"LDH",2,"A","a8"};unprefixed[0xF1]={true,"POP",1,"AF","*"};unprefixed[0xF2]={true,"LD",1,"A","C"};unprefixed[0xF3]={true,"DI",1,"*","*"};unprefixed[0xF4]={true,"ILLEGAL_F4",1,"*","*"};unprefixed[0xF5]={true,"PUSH",1,"AF","*"};unprefixed[0xF6]={true,"OR",2,"A","n8"};unprefixed[0xF7]={true,"RST",1,"$30","*"};unprefixed[0xF8]={true,"LD",2,"HL","SP"};  
}


int get_instruction_length(unsigned char opcode, bool cb_prefixed){
  OpCode * opcode_set = get_opcode_set(cb_prefixed);  
  return opcode_set[opcode].bytes;
}

void get_mnemonic(unsigned char opcode, bool cb_prefixed, char * s){
  OpCode * opcode_set = get_opcode_set(cb_prefixed); 
  strcpy(s, opcode_set[opcode].mnemonic);
}

bool operand_is_literal(char operand[4]){
  if (strlen(operand) == 1) {
    return true;
  } else if (!strcmp(operand, "AF")){
    return true;
  } else if (!strcmp(operand, "BC")){
    return true;
  } else if (!strcmp(operand, "DE")){
    return true;
  } else if (!strcmp(operand, "HL")){
    return true;
  } else if (!strcmp(operand, "SP")){
    return true;
  } else if (!strcmp(operand, "PC")){
    return true;
  } else if (!strcmp(operand, "NZ")){
    return true;
  } else if (!strcmp(operand, "NC")){
    return true;
  } else {
    return false;
  }
}

void get_reference(char * operand, int curr, char * buffer, char * s){
  char * tmp = (char *) malloc(sizeof(char)*6);
  u_int16_t bit_ref = 0;
  if (!strcmp(operand,"n8")){
    sprintf(tmp, "0x%02x", (unsigned char) buffer[curr+1]);
    strcat(s,tmp);
  } else if (!strcmp(operand,"a8")){
    sprintf(tmp, "$0x%02x", (unsigned char) buffer[curr+1]);
    strcat(s,tmp);
  } else if (!strcmp(operand,"e8")){
    sprintf(tmp, "0x%02xs", (unsigned char) buffer[curr+1]);
    strcat(s,tmp);
  // 16 bit references n16, a16
  } else if (!strcmp(operand,"n16")){
    bit_ref = (((unsigned char) buffer[curr+1] << 8) & 0xff00) | ((unsigned char) buffer[curr+2] & 0x00ff);
    sprintf(tmp, "0x%04x", bit_ref);
    strcat(s,tmp);
  } else if (!strcmp(operand,"a16")){
    bit_ref = (((unsigned char) buffer[curr+1] << 8) & 0xff00) | ((unsigned char) buffer[curr+2] & 0x00ff);
    sprintf(tmp, "$0x%04x", bit_ref);
    strcat(s,tmp);
  }
  free(tmp);
}

void get_operands(unsigned char opcode, bool cb_prefixed, char * s, char * buffer, int curr){
  OpCode * opcode_set = get_opcode_set(cb_prefixed); 
  if (!strcmp(opcode_set[opcode].op1, "*")){
    return;
  } else {
    strcat(s, " ");
  }
  
  if (operand_is_literal(opcode_set[opcode].op1)) {
    strcat(s, opcode_set[opcode].op1);
  } else {
    get_reference(opcode_set[opcode].op1, curr, buffer, s);
  }

  if (!strcmp(opcode_set[opcode].op2, "*")){
    return;
  } else {
    strcat(s, " ");
  }
  if (operand_is_literal(opcode_set[opcode].op2)) {
      strcat(s, opcode_set[opcode].op2);
  } else {
    get_reference(opcode_set[opcode].op2, curr, buffer, s);
  }
}

bool is_instruction(unsigned char opcode, bool cb_prefixed){
  if(cb_prefixed == true){
    return cbprefixed[opcode].valid;
  } else {
    return unprefixed[opcode].valid;
  }
}

void fetch(uint8_t *buffer, int &index,  

int parse_instruction(unsigned char * buffer, int curr, char * output, int * line){

  bool cb_prefixed = (unsigned char) buffer[curr] == 0xCB;
  curr = curr + cb_prefixed;
  unsigned char opcode = buffer[curr];

  if (!is_instruction(opcode, cb_prefixed)) {return curr + 1;}

  int instruction_length = get_instruction_length(opcode, cb_prefixed);
  //new line
  (*line)++;

  get_mnemonic(opcode, cb_prefixed, output);
  get_operands(opcode, cb_prefixed, output, buffer, curr);
  return curr + instruction_length;
}

int main(int argc, char **argv) {

    init_opcodes();
    
    std::ifstream is ("test.gb", std::ifstream::binary);
    //get length of ROM
    is.seekg(0, is.end);
    int length = is.tellg();
    is.seekg(0, is.beg);

    //read into buffer
    unsigned char * buffer = new unsigned char [length];
    std::cout << "Reading " << length << " bytes\n";
    is.read( (char*)(&buffer[0]), length);
    if (is)
      std::cout << "Read ROM\n";
    else
      std::cout << "Error: only " << is.gcount() << " could be read";
    is.close();

    CartridgeHeader ch = CartridgeHeader(buffer, length);
    ch.display_header_data();

    int i = ROM_START;
    int l = 0;

    while(i<length){
      i = parse_instruction(buffer, i, output[l], &l);
      std::cout<<output[l-1]<<std::endl;
    }

    delete[] buffer;
    return 0;
}

