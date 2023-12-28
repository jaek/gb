#include <cstring>
#include <iostream>
#include <format>

#include "instructions.h"

Operand::Operand()
{
  datatype = none;
  data8 = 0;
}

bool Operand::is16bit()
{
  return datatype >= n16;
}

void Operand::show()
{
  int data;
  if (is16bit())
  {
    data = data16;
  }
  else
  {
    data = data8;
  }
  std::cout << std::hex << data;
}

uint8_t Operand::get_offset()
{
  switch (datatype)
  {
  case n8:
  case a8:
  case s8:
    return 1;
  case bit:
  case n16:
  case a16:
    return 2;
  default:
    return 0;
  }
}

OpDataType get_operand_type(char *s)
{
  char c = s[0];
  if (strlen(s) == 1)
  {
    if (c >= '0' && c <= '9')
    {
      return bit;
    }
    else if (c == 'Z')
    {
      return zero;
    }
    else
    {
      return reg8;
    }
  }
  else if (strlen(s) == 2)
  {
    if (c >= 'a' and c <= 'z')
    { // literal
      switch (c)
      {
      case 'a':
        return a8;
      case 'n':
        return n8;
      default:
        return s8;
      }
    }
    else if (s[1] == 'Z')
    {
      return zero;
    }
    else
    {
      return reg16;
    }
  }
  else
  {
    switch (c)
    {
    case 'a':
      return a16;
    case 'n':
      return n16;
    case '$':
      return vector;
    default:
      return none;
    }
  }
  return none;
}

OpCode get_op(uint8_t oc)
{
  switch (oc)
  {
  case 0x00:
    return {0x00, "NOP", 1, '-', '-', '-', '-', 4, 0, "", 0, "", 0};
  case 0x01:
    return {0x01, "LD", 3, '-', '-', '-', '-', 12, 0, "BC", 1, "n16", 2};
  case 0x02:
    return {0x02, "LD", 1, '-', '-', '-', '-', 8, 0, "BC", 1, "A", 1};
  case 0x03:
    return {0x03, "INC", 1, '-', '-', '-', '-', 8, 0, "BC", 1, "", 0};
  case 0x04:
    return {0x04, "INC", 1, 'Z', '0', 'H', '-', 4, 0, "B", 1, "", 0};
  case 0x05:
    return {0x05, "DEC", 1, 'Z', '1', 'H', '-', 4, 0, "B", 1, "", 0};
  case 0x06:
    return {0x06, "LD", 2, '-', '-', '-', '-', 8, 0, "B", 1, "n8", 1};
  case 0x07:
    return {0x07, "RLCA", 1, '0', '0', '0', 'C', 4, 0, "", 0, "", 0};
  case 0x08:
    return {0x08, "LD", 3, '-', '-', '-', '-', 20, 0, "a16", 2, "SP", 1};
  case 0x09:
    return {0x09, "ADD", 1, '-', '0', 'H', 'C', 8, 0, "HL", 1, "BC", 1};
  case 0x0a:
    return {0x0a, "LD", 1, '-', '-', '-', '-', 8, 0, "A", 1, "BC", 1};
  case 0x0b:
    return {0x0b, "DEC", 1, '-', '-', '-', '-', 8, 0, "BC", 1, "", 0};
  case 0x0c:
    return {0x0c, "INC", 1, 'Z', '0', 'H', '-', 4, 0, "C", 1, "", 0};
  case 0x0d:
    return {0x0d, "DEC", 1, 'Z', '1', 'H', '-', 4, 0, "C", 1, "", 0};
  case 0x0e:
    return {0x0e, "LD", 2, '-', '-', '-', '-', 8, 0, "C", 1, "n8", 1};
  case 0x0f:
    return {0x0f, "RRCA", 1, '0', '0', '0', 'C', 4, 0, "", 0, "", 0};
  case 0x10:
    return {0x10, "STOP", 2, '-', '-', '-', '-', 4, 0, "n8", 1, "", 0};
  case 0x11:
    return {0x11, "LD", 3, '-', '-', '-', '-', 12, 0, "DE", 1, "n16", 2};
  case 0x12:
    return {0x12, "LD", 1, '-', '-', '-', '-', 8, 0, "DE", 1, "A", 1};
  case 0x13:
    return {0x13, "INC", 1, '-', '-', '-', '-', 8, 0, "DE", 1, "", 0};
  case 0x14:
    return {0x14, "INC", 1, 'Z', '0', 'H', '-', 4, 0, "D", 1, "", 0};
  case 0x15:
    return {0x15, "DEC", 1, 'Z', '1', 'H', '-', 4, 0, "D", 1, "", 0};
  case 0x16:
    return {0x16, "LD", 2, '-', '-', '-', '-', 8, 0, "D", 1, "n8", 1};
  case 0x17:
    return {0x17, "RLA", 1, '0', '0', '0', 'C', 4, 0, "", 0, "", 0};
  case 0x18:
    return {0x18, "JR", 2, '-', '-', '-', '-', 12, 0, "e8", 1, "", 0};
  case 0x19:
    return {0x19, "ADD", 1, '-', '0', 'H', 'C', 8, 0, "HL", 1, "DE", 1};
  case 0x1a:
    return {0x1a, "LD", 1, '-', '-', '-', '-', 8, 0, "A", 1, "DE", 1};
  case 0x1b:
    return {0x1b, "DEC", 1, '-', '-', '-', '-', 8, 0, "DE", 1, "", 0};
  case 0x1c:
    return {0x1c, "INC", 1, 'Z', '0', 'H', '-', 4, 0, "E", 1, "", 0};
  case 0x1d:
    return {0x1d, "DEC", 1, 'Z', '1', 'H', '-', 4, 0, "E", 1, "", 0};
  case 0x1e:
    return {0x1e, "LD", 2, '-', '-', '-', '-', 8, 0, "E", 1, "n8", 1};
  case 0x1f:
    return {0x1f, "RRA", 1, '0', '0', '0', 'C', 4, 0, "", 0, "", 0};
  case 0x20:
    return {0x20, "JR", 2, '-', '-', '-', '-', 12, 8, "NZ", 1, "e8", 1};
  case 0x21:
    return {0x21, "LD", 3, '-', '-', '-', '-', 12, 0, "HL", 1, "n16", 2};
  case 0x22:
    return {0x22, "LD", 1, '-', '-', '-', '-', 8, 0, "HL", 1, "A", 1};
  case 0x23:
    return {0x23, "INC", 1, '-', '-', '-', '-', 8, 0, "HL", 1, "", 0};
  case 0x24:
    return {0x24, "INC", 1, 'Z', '0', 'H', '-', 4, 0, "H", 1, "", 0};
  case 0x25:
    return {0x25, "DEC", 1, 'Z', '1', 'H', '-', 4, 0, "H", 1, "", 0};
  case 0x26:
    return {0x26, "LD", 2, '-', '-', '-', '-', 8, 0, "H", 1, "n8", 1};
  case 0x27:
    return {0x27, "DAA", 1, 'Z', '-', '0', 'C', 4, 0, "", 0, "", 0};
  case 0x28:
    return {0x28, "JR", 2, '-', '-', '-', '-', 12, 8, "Z", 1, "e8", 1};
  case 0x29:
    return {0x29, "ADD", 1, '-', '0', 'H', 'C', 8, 0, "HL", 1, "HL", 1};
  case 0x2a:
    return {0x2a, "LD", 1, '-', '-', '-', '-', 8, 0, "A", 1, "HL", 1};
  case 0x2b:
    return {0x2b, "DEC", 1, '-', '-', '-', '-', 8, 0, "HL", 1, "", 0};
  case 0x2c:
    return {0x2c, "INC", 1, 'Z', '0', 'H', '-', 4, 0, "L", 1, "", 0};
  case 0x2d:
    return {0x2d, "DEC", 1, 'Z', '1', 'H', '-', 4, 0, "L", 1, "", 0};
  case 0x2e:
    return {0x2e, "LD", 2, '-', '-', '-', '-', 8, 0, "L", 1, "n8", 1};
  case 0x2f:
    return {0x2f, "CPL", 1, '-', '1', '1', '-', 4, 0, "", 0, "", 0};
  case 0x30:
    return {0x30, "JR", 2, '-', '-', '-', '-', 12, 8, "NC", 1, "e8", 1};
  case 0x31:
    return {0x31, "LD", 3, '-', '-', '-', '-', 12, 0, "SP", 1, "n16", 2};
  case 0x32:
    return {0x32, "LD", 1, '-', '-', '-', '-', 8, 0, "HL", 1, "A", 1};
  case 0x33:
    return {0x33, "INC", 1, '-', '-', '-', '-', 8, 0, "SP", 1, "", 0};
  case 0x34:
    return {0x34, "INC", 1, 'Z', '0', 'H', '-', 12, 0, "HL", 1, "", 0};
  case 0x35:
    return {0x35, "DEC", 1, 'Z', '1', 'H', '-', 12, 0, "HL", 1, "", 0};
  case 0x36:
    return {0x36, "LD", 2, '-', '-', '-', '-', 12, 0, "HL", 1, "n8", 1};
  case 0x37:
    return {0x37, "SCF", 1, '-', '0', '0', '1', 4, 0, "", 0, "", 0};
  case 0x38:
    return {0x38, "JR", 2, '-', '-', '-', '-', 12, 8, "C", 1, "e8", 1};
  case 0x39:
    return {0x39, "ADD", 1, '-', '0', 'H', 'C', 8, 0, "HL", 1, "SP", 1};
  case 0x3a:
    return {0x3a, "LD", 1, '-', '-', '-', '-', 8, 0, "A", 1, "HL", 1};
  case 0x3b:
    return {0x3b, "DEC", 1, '-', '-', '-', '-', 8, 0, "SP", 1, "", 0};
  case 0x3c:
    return {0x3c, "INC", 1, 'Z', '0', 'H', '-', 4, 0, "A", 1, "", 0};
  case 0x3d:
    return {0x3d, "DEC", 1, 'Z', '1', 'H', '-', 4, 0, "A", 1, "", 0};
  case 0x3e:
    return {0x3e, "LD", 2, '-', '-', '-', '-', 8, 0, "A", 1, "n8", 1};
  case 0x3f:
    return {0x3f, "CCF", 1, '-', '0', '0', 'C', 4, 0, "", 0, "", 0};
  case 0x40:
    return {0x40, "LD", 1, '-', '-', '-', '-', 4, 0, "B", 1, "B", 1};
  case 0x41:
    return {0x41, "LD", 1, '-', '-', '-', '-', 4, 0, "B", 1, "C", 1};
  case 0x42:
    return {0x42, "LD", 1, '-', '-', '-', '-', 4, 0, "B", 1, "D", 1};
  case 0x43:
    return {0x43, "LD", 1, '-', '-', '-', '-', 4, 0, "B", 1, "E", 1};
  case 0x44:
    return {0x44, "LD", 1, '-', '-', '-', '-', 4, 0, "B", 1, "H", 1};
  case 0x45:
    return {0x45, "LD", 1, '-', '-', '-', '-', 4, 0, "B", 1, "L", 1};
  case 0x46:
    return {0x46, "LD", 1, '-', '-', '-', '-', 8, 0, "B", 1, "HL", 1};
  case 0x47:
    return {0x47, "LD", 1, '-', '-', '-', '-', 4, 0, "B", 1, "A", 1};
  case 0x48:
    return {0x48, "LD", 1, '-', '-', '-', '-', 4, 0, "C", 1, "B", 1};
  case 0x49:
    return {0x49, "LD", 1, '-', '-', '-', '-', 4, 0, "C", 1, "C", 1};
  case 0x4a:
    return {0x4a, "LD", 1, '-', '-', '-', '-', 4, 0, "C", 1, "D", 1};
  case 0x4b:
    return {0x4b, "LD", 1, '-', '-', '-', '-', 4, 0, "C", 1, "E", 1};
  case 0x4c:
    return {0x4c, "LD", 1, '-', '-', '-', '-', 4, 0, "C", 1, "H", 1};
  case 0x4d:
    return {0x4d, "LD", 1, '-', '-', '-', '-', 4, 0, "C", 1, "L", 1};
  case 0x4e:
    return {0x4e, "LD", 1, '-', '-', '-', '-', 8, 0, "C", 1, "HL", 1};
  case 0x4f:
    return {0x4f, "LD", 1, '-', '-', '-', '-', 4, 0, "C", 1, "A", 1};
  case 0x50:
    return {0x50, "LD", 1, '-', '-', '-', '-', 4, 0, "D", 1, "B", 1};
  case 0x51:
    return {0x51, "LD", 1, '-', '-', '-', '-', 4, 0, "D", 1, "C", 1};
  case 0x52:
    return {0x52, "LD", 1, '-', '-', '-', '-', 4, 0, "D", 1, "D", 1};
  case 0x53:
    return {0x53, "LD", 1, '-', '-', '-', '-', 4, 0, "D", 1, "E", 1};
  case 0x54:
    return {0x54, "LD", 1, '-', '-', '-', '-', 4, 0, "D", 1, "H", 1};
  case 0x55:
    return {0x55, "LD", 1, '-', '-', '-', '-', 4, 0, "D", 1, "L", 1};
  case 0x56:
    return {0x56, "LD", 1, '-', '-', '-', '-', 8, 0, "D", 1, "HL", 1};
  case 0x57:
    return {0x57, "LD", 1, '-', '-', '-', '-', 4, 0, "D", 1, "A", 1};
  case 0x58:
    return {0x58, "LD", 1, '-', '-', '-', '-', 4, 0, "E", 1, "B", 1};
  case 0x59:
    return {0x59, "LD", 1, '-', '-', '-', '-', 4, 0, "E", 1, "C", 1};
  case 0x5a:
    return {0x5a, "LD", 1, '-', '-', '-', '-', 4, 0, "E", 1, "D", 1};
  case 0x5b:
    return {0x5b, "LD", 1, '-', '-', '-', '-', 4, 0, "E", 1, "E", 1};
  case 0x5c:
    return {0x5c, "LD", 1, '-', '-', '-', '-', 4, 0, "E", 1, "H", 1};
  case 0x5d:
    return {0x5d, "LD", 1, '-', '-', '-', '-', 4, 0, "E", 1, "L", 1};
  case 0x5e:
    return {0x5e, "LD", 1, '-', '-', '-', '-', 8, 0, "E", 1, "HL", 1};
  case 0x5f:
    return {0x5f, "LD", 1, '-', '-', '-', '-', 4, 0, "E", 1, "A", 1};
  case 0x60:
    return {0x60, "LD", 1, '-', '-', '-', '-', 4, 0, "H", 1, "B", 1};
  case 0x61:
    return {0x61, "LD", 1, '-', '-', '-', '-', 4, 0, "H", 1, "C", 1};
  case 0x62:
    return {0x62, "LD", 1, '-', '-', '-', '-', 4, 0, "H", 1, "D", 1};
  case 0x63:
    return {0x63, "LD", 1, '-', '-', '-', '-', 4, 0, "H", 1, "E", 1};
  case 0x64:
    return {0x64, "LD", 1, '-', '-', '-', '-', 4, 0, "H", 1, "H", 1};
  case 0x65:
    return {0x65, "LD", 1, '-', '-', '-', '-', 4, 0, "H", 1, "L", 1};
  case 0x66:
    return {0x66, "LD", 1, '-', '-', '-', '-', 8, 0, "H", 1, "HL", 1};
  case 0x67:
    return {0x67, "LD", 1, '-', '-', '-', '-', 4, 0, "H", 1, "A", 1};
  case 0x68:
    return {0x68, "LD", 1, '-', '-', '-', '-', 4, 0, "L", 1, "B", 1};
  case 0x69:
    return {0x69, "LD", 1, '-', '-', '-', '-', 4, 0, "L", 1, "C", 1};
  case 0x6a:
    return {0x6a, "LD", 1, '-', '-', '-', '-', 4, 0, "L", 1, "D", 1};
  case 0x6b:
    return {0x6b, "LD", 1, '-', '-', '-', '-', 4, 0, "L", 1, "E", 1};
  case 0x6c:
    return {0x6c, "LD", 1, '-', '-', '-', '-', 4, 0, "L", 1, "H", 1};
  case 0x6d:
    return {0x6d, "LD", 1, '-', '-', '-', '-', 4, 0, "L", 1, "L", 1};
  case 0x6e:
    return {0x6e, "LD", 1, '-', '-', '-', '-', 8, 0, "L", 1, "HL", 1};
  case 0x6f:
    return {0x6f, "LD", 1, '-', '-', '-', '-', 4, 0, "L", 1, "A", 1};
  case 0x70:
    return {0x70, "LD", 1, '-', '-', '-', '-', 8, 0, "HL", 1, "B", 1};
  case 0x71:
    return {0x71, "LD", 1, '-', '-', '-', '-', 8, 0, "HL", 1, "C", 1};
  case 0x72:
    return {0x72, "LD", 1, '-', '-', '-', '-', 8, 0, "HL", 1, "D", 1};
  case 0x73:
    return {0x73, "LD", 1, '-', '-', '-', '-', 8, 0, "HL", 1, "E", 1};
  case 0x74:
    return {0x74, "LD", 1, '-', '-', '-', '-', 8, 0, "HL", 1, "H", 1};
  case 0x75:
    return {0x75, "LD", 1, '-', '-', '-', '-', 8, 0, "HL", 1, "L", 1};
  case 0x76:
    return {0x76, "HALT", 1, '-', '-', '-', '-', 4, 0, "", 0, "", 0};
  case 0x77:
    return {0x77, "LD", 1, '-', '-', '-', '-', 8, 0, "HL", 1, "A", 1};
  case 0x78:
    return {0x78, "LD", 1, '-', '-', '-', '-', 4, 0, "A", 1, "B", 1};
  case 0x79:
    return {0x79, "LD", 1, '-', '-', '-', '-', 4, 0, "A", 1, "C", 1};
  case 0x7a:
    return {0x7a, "LD", 1, '-', '-', '-', '-', 4, 0, "A", 1, "D", 1};
  case 0x7b:
    return {0x7b, "LD", 1, '-', '-', '-', '-', 4, 0, "A", 1, "E", 1};
  case 0x7c:
    return {0x7c, "LD", 1, '-', '-', '-', '-', 4, 0, "A", 1, "H", 1};
  case 0x7d:
    return {0x7d, "LD", 1, '-', '-', '-', '-', 4, 0, "A", 1, "L", 1};
  case 0x7e:
    return {0x7e, "LD", 1, '-', '-', '-', '-', 8, 0, "A", 1, "HL", 1};
  case 0x7f:
    return {0x7f, "LD", 1, '-', '-', '-', '-', 4, 0, "A", 1, "A", 1};
  case 0x80:
    return {0x80, "ADD", 1, 'Z', '0', 'H', 'C', 4, 0, "A", 1, "B", 1};
  case 0x81:
    return {0x81, "ADD", 1, 'Z', '0', 'H', 'C', 4, 0, "A", 1, "C", 1};
  case 0x82:
    return {0x82, "ADD", 1, 'Z', '0', 'H', 'C', 4, 0, "A", 1, "D", 1};
  case 0x83:
    return {0x83, "ADD", 1, 'Z', '0', 'H', 'C', 4, 0, "A", 1, "E", 1};
  case 0x84:
    return {0x84, "ADD", 1, 'Z', '0', 'H', 'C', 4, 0, "A", 1, "H", 1};
  case 0x85:
    return {0x85, "ADD", 1, 'Z', '0', 'H', 'C', 4, 0, "A", 1, "L", 1};
  case 0x86:
    return {0x86, "ADD", 1, 'Z', '0', 'H', 'C', 8, 0, "A", 1, "HL", 1};
  case 0x87:
    return {0x87, "ADD", 1, 'Z', '0', 'H', 'C', 4, 0, "A", 1, "A", 1};
  case 0x88:
    return {0x88, "ADC", 1, 'Z', '0', 'H', 'C', 4, 0, "A", 1, "B", 1};
  case 0x89:
    return {0x89, "ADC", 1, 'Z', '0', 'H', 'C', 4, 0, "A", 1, "C", 1};
  case 0x8a:
    return {0x8a, "ADC", 1, 'Z', '0', 'H', 'C', 4, 0, "A", 1, "D", 1};
  case 0x8b:
    return {0x8b, "ADC", 1, 'Z', '0', 'H', 'C', 4, 0, "A", 1, "E", 1};
  case 0x8c:
    return {0x8c, "ADC", 1, 'Z', '0', 'H', 'C', 4, 0, "A", 1, "H", 1};
  case 0x8d:
    return {0x8d, "ADC", 1, 'Z', '0', 'H', 'C', 4, 0, "A", 1, "L", 1};
  case 0x8e:
    return {0x8e, "ADC", 1, 'Z', '0', 'H', 'C', 8, 0, "A", 1, "HL", 1};
  case 0x8f:
    return {0x8f, "ADC", 1, 'Z', '0', 'H', 'C', 4, 0, "A", 1, "A", 1};
  case 0x90:
    return {0x90, "SUB", 1, 'Z', '1', 'H', 'C', 4, 0, "A", 1, "B", 1};
  case 0x91:
    return {0x91, "SUB", 1, 'Z', '1', 'H', 'C', 4, 0, "A", 1, "C", 1};
  case 0x92:
    return {0x92, "SUB", 1, 'Z', '1', 'H', 'C', 4, 0, "A", 1, "D", 1};
  case 0x93:
    return {0x93, "SUB", 1, 'Z', '1', 'H', 'C', 4, 0, "A", 1, "E", 1};
  case 0x94:
    return {0x94, "SUB", 1, 'Z', '1', 'H', 'C', 4, 0, "A", 1, "H", 1};
  case 0x95:
    return {0x95, "SUB", 1, 'Z', '1', 'H', 'C', 4, 0, "A", 1, "L", 1};
  case 0x96:
    return {0x96, "SUB", 1, 'Z', '1', 'H', 'C', 8, 0, "A", 1, "HL", 1};
  case 0x97:
    return {0x97, "SUB", 1, '1', '1', '0', '0', 4, 0, "A", 1, "A", 1};
  case 0x98:
    return {0x98, "SBC", 1, 'Z', '1', 'H', 'C', 4, 0, "A", 1, "B", 1};
  case 0x99:
    return {0x99, "SBC", 1, 'Z', '1', 'H', 'C', 4, 0, "A", 1, "C", 1};
  case 0x9a:
    return {0x9a, "SBC", 1, 'Z', '1', 'H', 'C', 4, 0, "A", 1, "D", 1};
  case 0x9b:
    return {0x9b, "SBC", 1, 'Z', '1', 'H', 'C', 4, 0, "A", 1, "E", 1};
  case 0x9c:
    return {0x9c, "SBC", 1, 'Z', '1', 'H', 'C', 4, 0, "A", 1, "H", 1};
  case 0x9d:
    return {0x9d, "SBC", 1, 'Z', '1', 'H', 'C', 4, 0, "A", 1, "L", 1};
  case 0x9e:
    return {0x9e, "SBC", 1, 'Z', '1', 'H', 'C', 8, 0, "A", 1, "HL", 1};
  case 0x9f:
    return {0x9f, "SBC", 1, 'Z', '1', 'H', '-', 4, 0, "A", 1, "A", 1};
  case 0xa0:
    return {0xa0, "AND", 1, 'Z', '0', '1', '0', 4, 0, "A", 1, "B", 1};
  case 0xa1:
    return {0xa1, "AND", 1, 'Z', '0', '1', '0', 4, 0, "A", 1, "C", 1};
  case 0xa2:
    return {0xa2, "AND", 1, 'Z', '0', '1', '0', 4, 0, "A", 1, "D", 1};
  case 0xa3:
    return {0xa3, "AND", 1, 'Z', '0', '1', '0', 4, 0, "A", 1, "E", 1};
  case 0xa4:
    return {0xa4, "AND", 1, 'Z', '0', '1', '0', 4, 0, "A", 1, "H", 1};
  case 0xa5:
    return {0xa5, "AND", 1, 'Z', '0', '1', '0', 4, 0, "A", 1, "L", 1};
  case 0xa6:
    return {0xa6, "AND", 1, 'Z', '0', '1', '0', 8, 0, "A", 1, "HL", 1};
  case 0xa7:
    return {0xa7, "AND", 1, 'Z', '0', '1', '0', 4, 0, "A", 1, "A", 1};
  case 0xa8:
    return {0xa8, "XOR", 1, 'Z', '0', '0', '0', 4, 0, "A", 1, "B", 1};
  case 0xa9:
    return {0xa9, "XOR", 1, 'Z', '0', '0', '0', 4, 0, "A", 1, "C", 1};
  case 0xaa:
    return {0xaa, "XOR", 1, 'Z', '0', '0', '0', 4, 0, "A", 1, "D", 1};
  case 0xab:
    return {0xab, "XOR", 1, 'Z', '0', '0', '0', 4, 0, "A", 1, "E", 1};
  case 0xac:
    return {0xac, "XOR", 1, 'Z', '0', '0', '0', 4, 0, "A", 1, "H", 1};
  case 0xad:
    return {0xad, "XOR", 1, 'Z', '0', '0', '0', 4, 0, "A", 1, "L", 1};
  case 0xae:
    return {0xae, "XOR", 1, 'Z', '0', '0', '0', 8, 0, "A", 1, "HL", 1};
  case 0xaf:
    return {0xaf, "XOR", 1, '1', '0', '0', '0', 4, 0, "A", 1, "A", 1};
  case 0xb0:
    return {0xb0, "OR", 1, 'Z', '0', '0', '0', 4, 0, "A", 1, "B", 1};
  case 0xb1:
    return {0xb1, "OR", 1, 'Z', '0', '0', '0', 4, 0, "A", 1, "C", 1};
  case 0xb2:
    return {0xb2, "OR", 1, 'Z', '0', '0', '0', 4, 0, "A", 1, "D", 1};
  case 0xb3:
    return {0xb3, "OR", 1, 'Z', '0', '0', '0', 4, 0, "A", 1, "E", 1};
  case 0xb4:
    return {0xb4, "OR", 1, 'Z', '0', '0', '0', 4, 0, "A", 1, "H", 1};
  case 0xb5:
    return {0xb5, "OR", 1, 'Z', '0', '0', '0', 4, 0, "A", 1, "L", 1};
  case 0xb6:
    return {0xb6, "OR", 1, 'Z', '0', '0', '0', 8, 0, "A", 1, "HL", 1};
  case 0xb7:
    return {0xb7, "OR", 1, 'Z', '0', '0', '0', 4, 0, "A", 1, "A", 1};
  case 0xb8:
    return {0xb8, "CP", 1, 'Z', '1', 'H', 'C', 4, 0, "A", 1, "B", 1};
  case 0xb9:
    return {0xb9, "CP", 1, 'Z', '1', 'H', 'C', 4, 0, "A", 1, "C", 1};
  case 0xba:
    return {0xba, "CP", 1, 'Z', '1', 'H', 'C', 4, 0, "A", 1, "D", 1};
  case 0xbb:
    return {0xbb, "CP", 1, 'Z', '1', 'H', 'C', 4, 0, "A", 1, "E", 1};
  case 0xbc:
    return {0xbc, "CP", 1, 'Z', '1', 'H', 'C', 4, 0, "A", 1, "H", 1};
  case 0xbd:
    return {0xbd, "CP", 1, 'Z', '1', 'H', 'C', 4, 0, "A", 1, "L", 1};
  case 0xbe:
    return {0xbe, "CP", 1, 'Z', '1', 'H', 'C', 8, 0, "A", 1, "HL", 1};
  case 0xbf:
    return {0xbf, "CP", 1, '1', '1', '0', '0', 4, 0, "A", 1, "A", 1};
  case 0xc0:
    return {0xc0, "RET", 1, '-', '-', '-', '-', 20, 8, "NZ", 1, "", 0};
  case 0xc1:
    return {0xc1, "POP", 1, '-', '-', '-', '-', 12, 0, "BC", 1, "", 0};
  case 0xc2:
    return {0xc2, "JP", 3, '-', '-', '-', '-', 16, 12, "NZ", 1, "a16", 2};
  case 0xc3:
    return {0xc3, "JP", 3, '-', '-', '-', '-', 16, 0, "a16", 2, "", 0};
  case 0xc4:
    return {0xc4, "CALL", 3, '-', '-', '-', '-', 24, 12, "NZ", 1, "a16", 2};
  case 0xc5:
    return {0xc5, "PUSH", 1, '-', '-', '-', '-', 16, 0, "BC", 1, "", 0};
  case 0xc6:
    return {0xc6, "ADD", 2, 'Z', '0', 'H', 'C', 8, 0, "A", 1, "n8", 1};
  case 0xc7:
    return {0xc7, "RST", 1, '-', '-', '-', '-', 16, 0, "$00", 1, "", 0};
  case 0xc8:
    return {0xc8, "RET", 1, '-', '-', '-', '-', 20, 8, "Z", 1, "", 0};
  case 0xc9:
    return {0xc9, "RET", 1, '-', '-', '-', '-', 16, 0, "", 0, "", 0};
  case 0xca:
    return {0xca, "JP", 3, '-', '-', '-', '-', 16, 12, "Z", 1, "a16", 2};
  case 0xcb:
    return {0xcb, "PREFIX", 1, '-', '-', '-', '-', 4, 0, "", 0, "", 0};
  case 0xcc:
    return {0xcc, "CALL", 3, '-', '-', '-', '-', 24, 12, "Z", 1, "a16", 2};
  case 0xcd:
    return {0xcd, "CALL", 3, '-', '-', '-', '-', 24, 0, "a16", 2, "", 0};
  case 0xce:
    return {0xce, "ADC", 2, 'Z', '0', 'H', 'C', 8, 0, "A", 1, "n8", 1};
  case 0xcf:
    return {0xcf, "RST", 1, '-', '-', '-', '-', 16, 0, "$08", 1, "", 0};
  case 0xd0:
    return {0xd0, "RET", 1, '-', '-', '-', '-', 20, 8, "NC", 1, "", 0};
  case 0xd1:
    return {0xd1, "POP", 1, '-', '-', '-', '-', 12, 0, "DE", 1, "", 0};
  case 0xd2:
    return {0xd2, "JP", 3, '-', '-', '-', '-', 16, 12, "NC", 1, "a16", 2};
  case 0xd3:
    return {0xd3, "ILLEGAL_D3", 1, '-', '-', '-', '-', 4, 0, "", 0, "", 0};
  case 0xd4:
    return {0xd4, "CALL", 3, '-', '-', '-', '-', 24, 12, "NC", 1, "a16", 2};
  case 0xd5:
    return {0xd5, "PUSH", 1, '-', '-', '-', '-', 16, 0, "DE", 1, "", 0};
  case 0xd6:
    return {0xd6, "SUB", 2, 'Z', '1', 'H', 'C', 8, 0, "A", 1, "n8", 1};
  case 0xd7:
    return {0xd7, "RST", 1, '-', '-', '-', '-', 16, 0, "$10", 1, "", 0};
  case 0xd8:
    return {0xd8, "RET", 1, '-', '-', '-', '-', 20, 8, "C", 1, "", 0};
  case 0xd9:
    return {0xd9, "RETI", 1, '-', '-', '-', '-', 16, 0, "", 0, "", 0};
  case 0xda:
    return {0xda, "JP", 3, '-', '-', '-', '-', 16, 12, "C", 1, "a16", 2};
  case 0xdb:
    return {0xdb, "ILLEGAL_DB", 1, '-', '-', '-', '-', 4, 0, "", 0, "", 0};
  case 0xdc:
    return {0xdc, "CALL", 3, '-', '-', '-', '-', 24, 12, "C", 1, "a16", 2};
  case 0xdd:
    return {0xdd, "ILLEGAL_DD", 1, '-', '-', '-', '-', 4, 0, "", 0, "", 0};
  case 0xde:
    return {0xde, "SBC", 2, 'Z', '1', 'H', 'C', 8, 0, "A", 1, "n8", 1};
  case 0xdf:
    return {0xdf, "RST", 1, '-', '-', '-', '-', 16, 0, "$18", 1, "", 0};
  case 0xe0:
    return {0xe0, "LDH", 2, '-', '-', '-', '-', 12, 0, "a8", 1, "A", 1};
  case 0xe1:
    return {0xe1, "POP", 1, '-', '-', '-', '-', 12, 0, "HL", 1, "", 0};
  case 0xe2:
    return {0xe2, "LD", 1, '-', '-', '-', '-', 8, 0, "C", 1, "A", 1};
  case 0xe3:
    return {0xe3, "ILLEGAL_E3", 1, '-', '-', '-', '-', 4, 0, "", 0, "", 0};
  case 0xe4:
    return {0xe4, "ILLEGAL_E4", 1, '-', '-', '-', '-', 4, 0, "", 0, "", 0};
  case 0xe5:
    return {0xe5, "PUSH", 1, '-', '-', '-', '-', 16, 0, "HL", 1, "", 0};
  case 0xe6:
    return {0xe6, "AND", 2, 'Z', '0', '1', '0', 8, 0, "A", 1, "n8", 1};
  case 0xe7:
    return {0xe7, "RST", 1, '-', '-', '-', '-', 16, 0, "$20", 1, "", 0};
  case 0xe8:
    return {0xe8, "ADD", 2, '0', '0', 'H', 'C', 16, 0, "SP", 1, "e8", 1};
  case 0xe9:
    return {0xe9, "JP", 1, '-', '-', '-', '-', 4, 0, "HL", 1, "", 0};
  case 0xea:
    return {0xea, "LD", 3, '-', '-', '-', '-', 16, 0, "a16", 2, "A", 1};
  case 0xeb:
    return {0xeb, "ILLEGAL_EB", 1, '-', '-', '-', '-', 4, 0, "", 0, "", 0};
  case 0xec:
    return {0xec, "ILLEGAL_EC", 1, '-', '-', '-', '-', 4, 0, "", 0, "", 0};
  case 0xed:
    return {0xed, "ILLEGAL_ED", 1, '-', '-', '-', '-', 4, 0, "", 0, "", 0};
  case 0xee:
    return {0xee, "XOR", 2, 'Z', '0', '0', '0', 8, 0, "A", 1, "n8", 1};
  case 0xef:
    return {0xef, "RST", 1, '-', '-', '-', '-', 16, 0, "$28", 1, "", 0};
  case 0xf0:
    return {0xf0, "LDH", 2, '-', '-', '-', '-', 12, 0, "A", 1, "a8", 1};
  case 0xf1:
    return {0xf1, "POP", 1, 'Z', 'N', 'H', 'C', 12, 0, "AF", 1, "", 0};
  case 0xf2:
    return {0xf2, "LD", 1, '-', '-', '-', '-', 8, 0, "A", 1, "C", 1};
  case 0xf3:
    return {0xf3, "DI", 1, '-', '-', '-', '-', 4, 0, "", 0, "", 0};
  case 0xf4:
    return {0xf4, "ILLEGAL_F4", 1, '-', '-', '-', '-', 4, 0, "", 0, "", 0};
  case 0xf5:
    return {0xf5, "PUSH", 1, '-', '-', '-', '-', 16, 0, "AF", 1, "", 0};
  case 0xf6:
    return {0xf6, "OR", 2, 'Z', '0', '0', '0', 8, 0, "A", 1, "n8", 1};
  case 0xf7:
    return {0xf7, "RST", 1, '-', '-', '-', '-', 16, 0, "$30", 1, "", 0};
  case 0xf8:
    return {0xf8, "LD", 2, '0', '0', 'H', 'C', 12, 0, "HL", 1, "SP", 1};
  case 0xf9:
    return {0xf9, "LD", 1, '-', '-', '-', '-', 8, 0, "SP", 1, "HL", 1};
  case 0xfa:
    return {0xfa, "LD", 3, '-', '-', '-', '-', 16, 0, "A", 1, "a16", 2};
  case 0xfb:
    return {0xfb, "EI", 1, '-', '-', '-', '-', 4, 0, "", 0, "", 0};
  case 0xfc:
    return {0xfc, "ILLEGAL_FC", 1, '-', '-', '-', '-', 4, 0, "", 0, "", 0};
  case 0xfd:
    return {0xfd, "ILLEGAL_FD", 1, '-', '-', '-', '-', 4, 0, "", 0, "", 0};
  case 0xfe:
    return {0xfe, "CP", 2, 'Z', '1', 'H', 'C', 8, 0, "A", 1, "n8", 1};
  case 0xff:
    return {0xff, "RST", 1, '-', '-', '-', '-', 16, 0, "$38", 1, "", 0};
  default:
    throw std::invalid_argument(
        std::format(
            "get_op: invalid opcode"));
  }
}

OpCode get_cb_op(uint8_t oc)
{
  switch (oc)
  {
  case 0x00:
    return {0x00, "RLC", 2, 'Z', '0', '0', 'C', 8, 0, "B", 1, "", 0};
  case 0x01:
    return {0x01, "RLC", 2, 'Z', '0', '0', 'C', 8, 0, "C", 1, "", 0};
  case 0x02:
    return {0x02, "RLC", 2, 'Z', '0', '0', 'C', 8, 0, "D", 1, "", 0};
  case 0x03:
    return {0x03, "RLC", 2, 'Z', '0', '0', 'C', 8, 0, "E", 1, "", 0};
  case 0x04:
    return {0x04, "RLC", 2, 'Z', '0', '0', 'C', 8, 0, "H", 1, "", 0};
  case 0x05:
    return {0x05, "RLC", 2, 'Z', '0', '0', 'C', 8, 0, "L", 1, "", 0};
  case 0x06:
    return {0x06, "RLC", 2, 'Z', '0', '0', 'C', 16, 0, "HL", 1, "", 0};
  case 0x07:
    return {0x07, "RLC", 2, 'Z', '0', '0', 'C', 8, 0, "A", 1, "", 0};
  case 0x08:
    return {0x08, "RRC", 2, 'Z', '0', '0', 'C', 8, 0, "B", 1, "", 0};
  case 0x09:
    return {0x09, "RRC", 2, 'Z', '0', '0', 'C', 8, 0, "C", 1, "", 0};
  case 0x0a:
    return {0x0a, "RRC", 2, 'Z', '0', '0', 'C', 8, 0, "D", 1, "", 0};
  case 0x0b:
    return {0x0b, "RRC", 2, 'Z', '0', '0', 'C', 8, 0, "E", 1, "", 0};
  case 0x0c:
    return {0x0c, "RRC", 2, 'Z', '0', '0', 'C', 8, 0, "H", 1, "", 0};
  case 0x0d:
    return {0x0d, "RRC", 2, 'Z', '0', '0', 'C', 8, 0, "L", 1, "", 0};
  case 0x0e:
    return {0x0e, "RRC", 2, 'Z', '0', '0', 'C', 16, 0, "HL", 1, "", 0};
  case 0x0f:
    return {0x0f, "RRC", 2, 'Z', '0', '0', 'C', 8, 0, "A", 1, "", 0};
  case 0x10:
    return {0x10, "RL", 2, 'Z', '0', '0', 'C', 8, 0, "B", 1, "", 0};
  case 0x11:
    return {0x11, "RL", 2, 'Z', '0', '0', 'C', 8, 0, "C", 1, "", 0};
  case 0x12:
    return {0x12, "RL", 2, 'Z', '0', '0', 'C', 8, 0, "D", 1, "", 0};
  case 0x13:
    return {0x13, "RL", 2, 'Z', '0', '0', 'C', 8, 0, "E", 1, "", 0};
  case 0x14:
    return {0x14, "RL", 2, 'Z', '0', '0', 'C', 8, 0, "H", 1, "", 0};
  case 0x15:
    return {0x15, "RL", 2, 'Z', '0', '0', 'C', 8, 0, "L", 1, "", 0};
  case 0x16:
    return {0x16, "RL", 2, 'Z', '0', '0', 'C', 16, 0, "HL", 1, "", 0};
  case 0x17:
    return {0x17, "RL", 2, 'Z', '0', '0', 'C', 8, 0, "A", 1, "", 0};
  case 0x18:
    return {0x18, "RR", 2, 'Z', '0', '0', 'C', 8, 0, "B", 1, "", 0};
  case 0x19:
    return {0x19, "RR", 2, 'Z', '0', '0', 'C', 8, 0, "C", 1, "", 0};
  case 0x1a:
    return {0x1a, "RR", 2, 'Z', '0', '0', 'C', 8, 0, "D", 1, "", 0};
  case 0x1b:
    return {0x1b, "RR", 2, 'Z', '0', '0', 'C', 8, 0, "E", 1, "", 0};
  case 0x1c:
    return {0x1c, "RR", 2, 'Z', '0', '0', 'C', 8, 0, "H", 1, "", 0};
  case 0x1d:
    return {0x1d, "RR", 2, 'Z', '0', '0', 'C', 8, 0, "L", 1, "", 0};
  case 0x1e:
    return {0x1e, "RR", 2, 'Z', '0', '0', 'C', 16, 0, "HL", 1, "", 0};
  case 0x1f:
    return {0x1f, "RR", 2, 'Z', '0', '0', 'C', 8, 0, "A", 1, "", 0};
  case 0x20:
    return {0x20, "SLA", 2, 'Z', '0', '0', 'C', 8, 0, "B", 1, "", 0};
  case 0x21:
    return {0x21, "SLA", 2, 'Z', '0', '0', 'C', 8, 0, "C", 1, "", 0};
  case 0x22:
    return {0x22, "SLA", 2, 'Z', '0', '0', 'C', 8, 0, "D", 1, "", 0};
  case 0x23:
    return {0x23, "SLA", 2, 'Z', '0', '0', 'C', 8, 0, "E", 1, "", 0};
  case 0x24:
    return {0x24, "SLA", 2, 'Z', '0', '0', 'C', 8, 0, "H", 1, "", 0};
  case 0x25:
    return {0x25, "SLA", 2, 'Z', '0', '0', 'C', 8, 0, "L", 1, "", 0};
  case 0x26:
    return {0x26, "SLA", 2, 'Z', '0', '0', 'C', 16, 0, "HL", 1, "", 0};
  case 0x27:
    return {0x27, "SLA", 2, 'Z', '0', '0', 'C', 8, 0, "A", 1, "", 0};
  case 0x28:
    return {0x28, "SRA", 2, 'Z', '0', '0', 'C', 8, 0, "B", 1, "", 0};
  case 0x29:
    return {0x29, "SRA", 2, 'Z', '0', '0', 'C', 8, 0, "C", 1, "", 0};
  case 0x2a:
    return {0x2a, "SRA", 2, 'Z', '0', '0', 'C', 8, 0, "D", 1, "", 0};
  case 0x2b:
    return {0x2b, "SRA", 2, 'Z', '0', '0', 'C', 8, 0, "E", 1, "", 0};
  case 0x2c:
    return {0x2c, "SRA", 2, 'Z', '0', '0', 'C', 8, 0, "H", 1, "", 0};
  case 0x2d:
    return {0x2d, "SRA", 2, 'Z', '0', '0', 'C', 8, 0, "L", 1, "", 0};
  case 0x2e:
    return {0x2e, "SRA", 2, 'Z', '0', '0', 'C', 16, 0, "HL", 1, "", 0};
  case 0x2f:
    return {0x2f, "SRA", 2, 'Z', '0', '0', 'C', 8, 0, "A", 1, "", 0};
  case 0x30:
    return {0x30, "SWAP", 2, 'Z', '0', '0', '0', 8, 0, "B", 1, "", 0};
  case 0x31:
    return {0x31, "SWAP", 2, 'Z', '0', '0', '0', 8, 0, "C", 1, "", 0};
  case 0x32:
    return {0x32, "SWAP", 2, 'Z', '0', '0', '0', 8, 0, "D", 1, "", 0};
  case 0x33:
    return {0x33, "SWAP", 2, 'Z', '0', '0', '0', 8, 0, "E", 1, "", 0};
  case 0x34:
    return {0x34, "SWAP", 2, 'Z', '0', '0', '0', 8, 0, "H", 1, "", 0};
  case 0x35:
    return {0x35, "SWAP", 2, 'Z', '0', '0', '0', 8, 0, "L", 1, "", 0};
  case 0x36:
    return {0x36, "SWAP", 2, 'Z', '0', '0', '0', 16, 0, "HL", 1, "", 0};
  case 0x37:
    return {0x37, "SWAP", 2, 'Z', '0', '0', '0', 8, 0, "A", 1, "", 0};
  case 0x38:
    return {0x38, "SRL", 2, 'Z', '0', '0', 'C', 8, 0, "B", 1, "", 0};
  case 0x39:
    return {0x39, "SRL", 2, 'Z', '0', '0', 'C', 8, 0, "C", 1, "", 0};
  case 0x3a:
    return {0x3a, "SRL", 2, 'Z', '0', '0', 'C', 8, 0, "D", 1, "", 0};
  case 0x3b:
    return {0x3b, "SRL", 2, 'Z', '0', '0', 'C', 8, 0, "E", 1, "", 0};
  case 0x3c:
    return {0x3c, "SRL", 2, 'Z', '0', '0', 'C', 8, 0, "H", 1, "", 0};
  case 0x3d:
    return {0x3d, "SRL", 2, 'Z', '0', '0', 'C', 8, 0, "L", 1, "", 0};
  case 0x3e:
    return {0x3e, "SRL", 2, 'Z', '0', '0', 'C', 16, 0, "HL", 1, "", 0};
  case 0x3f:
    return {0x3f, "SRL", 2, 'Z', '0', '0', 'C', 8, 0, "A", 1, "", 0};
  case 0x40:
    return {0x40, "BIT", 2, 'Z', '0', '1', '-', 8, 0, "0", 1, "B", 1};
  case 0x41:
    return {0x41, "BIT", 2, 'Z', '0', '1', '-', 8, 0, "0", 1, "C", 1};
  case 0x42:
    return {0x42, "BIT", 2, 'Z', '0', '1', '-', 8, 0, "0", 1, "D", 1};
  case 0x43:
    return {0x43, "BIT", 2, 'Z', '0', '1', '-', 8, 0, "0", 1, "E", 1};
  case 0x44:
    return {0x44, "BIT", 2, 'Z', '0', '1', '-', 8, 0, "0", 1, "H", 1};
  case 0x45:
    return {0x45, "BIT", 2, 'Z', '0', '1', '-', 8, 0, "0", 1, "L", 1};
  case 0x46:
    return {0x46, "BIT", 2, 'Z', '0', '1', '-', 12, 0, "0", 1, "HL", 1};
  case 0x47:
    return {0x47, "BIT", 2, 'Z', '0', '1', '-', 8, 0, "0", 1, "A", 1};
  case 0x48:
    return {0x48, "BIT", 2, 'Z', '0', '1', '-', 8, 0, "1", 1, "B", 1};
  case 0x49:
    return {0x49, "BIT", 2, 'Z', '0', '1', '-', 8, 0, "1", 1, "C", 1};
  case 0x4a:
    return {0x4a, "BIT", 2, 'Z', '0', '1', '-', 8, 0, "1", 1, "D", 1};
  case 0x4b:
    return {0x4b, "BIT", 2, 'Z', '0', '1', '-', 8, 0, "1", 1, "E", 1};
  case 0x4c:
    return {0x4c, "BIT", 2, 'Z', '0', '1', '-', 8, 0, "1", 1, "H", 1};
  case 0x4d:
    return {0x4d, "BIT", 2, 'Z', '0', '1', '-', 8, 0, "1", 1, "L", 1};
  case 0x4e:
    return {0x4e, "BIT", 2, 'Z', '0', '1', '-', 12, 0, "1", 1, "HL", 1};
  case 0x4f:
    return {0x4f, "BIT", 2, 'Z', '0', '1', '-', 8, 0, "1", 1, "A", 1};
  case 0x50:
    return {0x50, "BIT", 2, 'Z', '0', '1', '-', 8, 0, "2", 1, "B", 1};
  case 0x51:
    return {0x51, "BIT", 2, 'Z', '0', '1', '-', 8, 0, "2", 1, "C", 1};
  case 0x52:
    return {0x52, "BIT", 2, 'Z', '0', '1', '-', 8, 0, "2", 1, "D", 1};
  case 0x53:
    return {0x53, "BIT", 2, 'Z', '0', '1', '-', 8, 0, "2", 1, "E", 1};
  case 0x54:
    return {0x54, "BIT", 2, 'Z', '0', '1', '-', 8, 0, "2", 1, "H", 1};
  case 0x55:
    return {0x55, "BIT", 2, 'Z', '0', '1', '-', 8, 0, "2", 1, "L", 1};
  case 0x56:
    return {0x56, "BIT", 2, 'Z', '0', '1', '-', 12, 0, "2", 1, "HL", 1};
  case 0x57:
    return {0x57, "BIT", 2, 'Z', '0', '1', '-', 8, 0, "2", 1, "A", 1};
  case 0x58:
    return {0x58, "BIT", 2, 'Z', '0', '1', '-', 8, 0, "3", 1, "B", 1};
  case 0x59:
    return {0x59, "BIT", 2, 'Z', '0', '1', '-', 8, 0, "3", 1, "C", 1};
  case 0x5a:
    return {0x5a, "BIT", 2, 'Z', '0', '1', '-', 8, 0, "3", 1, "D", 1};
  case 0x5b:
    return {0x5b, "BIT", 2, 'Z', '0', '1', '-', 8, 0, "3", 1, "E", 1};
  case 0x5c:
    return {0x5c, "BIT", 2, 'Z', '0', '1', '-', 8, 0, "3", 1, "H", 1};
  case 0x5d:
    return {0x5d, "BIT", 2, 'Z', '0', '1', '-', 8, 0, "3", 1, "L", 1};
  case 0x5e:
    return {0x5e, "BIT", 2, 'Z', '0', '1', '-', 12, 0, "3", 1, "HL", 1};
  case 0x5f:
    return {0x5f, "BIT", 2, 'Z', '0', '1', '-', 8, 0, "3", 1, "A", 1};
  case 0x60:
    return {0x60, "BIT", 2, 'Z', '0', '1', '-', 8, 0, "4", 1, "B", 1};
  case 0x61:
    return {0x61, "BIT", 2, 'Z', '0', '1', '-', 8, 0, "4", 1, "C", 1};
  case 0x62:
    return {0x62, "BIT", 2, 'Z', '0', '1', '-', 8, 0, "4", 1, "D", 1};
  case 0x63:
    return {0x63, "BIT", 2, 'Z', '0', '1', '-', 8, 0, "4", 1, "E", 1};
  case 0x64:
    return {0x64, "BIT", 2, 'Z', '0', '1', '-', 8, 0, "4", 1, "H", 1};
  case 0x65:
    return {0x65, "BIT", 2, 'Z', '0', '1', '-', 8, 0, "4", 1, "L", 1};
  case 0x66:
    return {0x66, "BIT", 2, 'Z', '0', '1', '-', 12, 0, "4", 1, "HL", 1};
  case 0x67:
    return {0x67, "BIT", 2, 'Z', '0', '1', '-', 8, 0, "4", 1, "A", 1};
  case 0x68:
    return {0x68, "BIT", 2, 'Z', '0', '1', '-', 8, 0, "5", 1, "B", 1};
  case 0x69:
    return {0x69, "BIT", 2, 'Z', '0', '1', '-', 8, 0, "5", 1, "C", 1};
  case 0x6a:
    return {0x6a, "BIT", 2, 'Z', '0', '1', '-', 8, 0, "5", 1, "D", 1};
  case 0x6b:
    return {0x6b, "BIT", 2, 'Z', '0', '1', '-', 8, 0, "5", 1, "E", 1};
  case 0x6c:
    return {0x6c, "BIT", 2, 'Z', '0', '1', '-', 8, 0, "5", 1, "H", 1};
  case 0x6d:
    return {0x6d, "BIT", 2, 'Z', '0', '1', '-', 8, 0, "5", 1, "L", 1};
  case 0x6e:
    return {0x6e, "BIT", 2, 'Z', '0', '1', '-', 12, 0, "5", 1, "HL", 1};
  case 0x6f:
    return {0x6f, "BIT", 2, 'Z', '0', '1', '-', 8, 0, "5", 1, "A", 1};
  case 0x70:
    return {0x70, "BIT", 2, 'Z', '0', '1', '-', 8, 0, "6", 1, "B", 1};
  case 0x71:
    return {0x71, "BIT", 2, 'Z', '0', '1', '-', 8, 0, "6", 1, "C", 1};
  case 0x72:
    return {0x72, "BIT", 2, 'Z', '0', '1', '-', 8, 0, "6", 1, "D", 1};
  case 0x73:
    return {0x73, "BIT", 2, 'Z', '0', '1', '-', 8, 0, "6", 1, "E", 1};
  case 0x74:
    return {0x74, "BIT", 2, 'Z', '0', '1', '-', 8, 0, "6", 1, "H", 1};
  case 0x75:
    return {0x75, "BIT", 2, 'Z', '0', '1', '-', 8, 0, "6", 1, "L", 1};
  case 0x76:
    return {0x76, "BIT", 2, 'Z', '0', '1', '-', 12, 0, "6", 1, "HL", 1};
  case 0x77:
    return {0x77, "BIT", 2, 'Z', '0', '1', '-', 8, 0, "6", 1, "A", 1};
  case 0x78:
    return {0x78, "BIT", 2, 'Z', '0', '1', '-', 8, 0, "7", 1, "B", 1};
  case 0x79:
    return {0x79, "BIT", 2, 'Z', '0', '1', '-', 8, 0, "7", 1, "C", 1};
  case 0x7a:
    return {0x7a, "BIT", 2, 'Z', '0', '1', '-', 8, 0, "7", 1, "D", 1};
  case 0x7b:
    return {0x7b, "BIT", 2, 'Z', '0', '1', '-', 8, 0, "7", 1, "E", 1};
  case 0x7c:
    return {0x7c, "BIT", 2, 'Z', '0', '1', '-', 8, 0, "7", 1, "H", 1};
  case 0x7d:
    return {0x7d, "BIT", 2, 'Z', '0', '1', '-', 8, 0, "7", 1, "L", 1};
  case 0x7e:
    return {0x7e, "BIT", 2, 'Z', '0', '1', '-', 12, 0, "7", 1, "HL", 1};
  case 0x7f:
    return {0x7f, "BIT", 2, 'Z', '0', '1', '-', 8, 0, "7", 1, "A", 1};
  case 0x80:
    return {0x80, "RES", 2, '-', '-', '-', '-', 8, 0, "0", 1, "B", 1};
  case 0x81:
    return {0x81, "RES", 2, '-', '-', '-', '-', 8, 0, "0", 1, "C", 1};
  case 0x82:
    return {0x82, "RES", 2, '-', '-', '-', '-', 8, 0, "0", 1, "D", 1};
  case 0x83:
    return {0x83, "RES", 2, '-', '-', '-', '-', 8, 0, "0", 1, "E", 1};
  case 0x84:
    return {0x84, "RES", 2, '-', '-', '-', '-', 8, 0, "0", 1, "H", 1};
  case 0x85:
    return {0x85, "RES", 2, '-', '-', '-', '-', 8, 0, "0", 1, "L", 1};
  case 0x86:
    return {0x86, "RES", 2, '-', '-', '-', '-', 16, 0, "0", 1, "HL", 1};
  case 0x87:
    return {0x87, "RES", 2, '-', '-', '-', '-', 8, 0, "0", 1, "A", 1};
  case 0x88:
    return {0x88, "RES", 2, '-', '-', '-', '-', 8, 0, "1", 1, "B", 1};
  case 0x89:
    return {0x89, "RES", 2, '-', '-', '-', '-', 8, 0, "1", 1, "C", 1};
  case 0x8a:
    return {0x8a, "RES", 2, '-', '-', '-', '-', 8, 0, "1", 1, "D", 1};
  case 0x8b:
    return {0x8b, "RES", 2, '-', '-', '-', '-', 8, 0, "1", 1, "E", 1};
  case 0x8c:
    return {0x8c, "RES", 2, '-', '-', '-', '-', 8, 0, "1", 1, "H", 1};
  case 0x8d:
    return {0x8d, "RES", 2, '-', '-', '-', '-', 8, 0, "1", 1, "L", 1};
  case 0x8e:
    return {0x8e, "RES", 2, '-', '-', '-', '-', 16, 0, "1", 1, "HL", 1};
  case 0x8f:
    return {0x8f, "RES", 2, '-', '-', '-', '-', 8, 0, "1", 1, "A", 1};
  case 0x90:
    return {0x90, "RES", 2, '-', '-', '-', '-', 8, 0, "2", 1, "B", 1};
  case 0x91:
    return {0x91, "RES", 2, '-', '-', '-', '-', 8, 0, "2", 1, "C", 1};
  case 0x92:
    return {0x92, "RES", 2, '-', '-', '-', '-', 8, 0, "2", 1, "D", 1};
  case 0x93:
    return {0x93, "RES", 2, '-', '-', '-', '-', 8, 0, "2", 1, "E", 1};
  case 0x94:
    return {0x94, "RES", 2, '-', '-', '-', '-', 8, 0, "2", 1, "H", 1};
  case 0x95:
    return {0x95, "RES", 2, '-', '-', '-', '-', 8, 0, "2", 1, "L", 1};
  case 0x96:
    return {0x96, "RES", 2, '-', '-', '-', '-', 16, 0, "2", 1, "HL", 1};
  case 0x97:
    return {0x97, "RES", 2, '-', '-', '-', '-', 8, 0, "2", 1, "A", 1};
  case 0x98:
    return {0x98, "RES", 2, '-', '-', '-', '-', 8, 0, "3", 1, "B", 1};
  case 0x99:
    return {0x99, "RES", 2, '-', '-', '-', '-', 8, 0, "3", 1, "C", 1};
  case 0x9a:
    return {0x9a, "RES", 2, '-', '-', '-', '-', 8, 0, "3", 1, "D", 1};
  case 0x9b:
    return {0x9b, "RES", 2, '-', '-', '-', '-', 8, 0, "3", 1, "E", 1};
  case 0x9c:
    return {0x9c, "RES", 2, '-', '-', '-', '-', 8, 0, "3", 1, "H", 1};
  case 0x9d:
    return {0x9d, "RES", 2, '-', '-', '-', '-', 8, 0, "3", 1, "L", 1};
  case 0x9e:
    return {0x9e, "RES", 2, '-', '-', '-', '-', 16, 0, "3", 1, "HL", 1};
  case 0x9f:
    return {0x9f, "RES", 2, '-', '-', '-', '-', 8, 0, "3", 1, "A", 1};
  case 0xa0:
    return {0xa0, "RES", 2, '-', '-', '-', '-', 8, 0, "4", 1, "B", 1};
  case 0xa1:
    return {0xa1, "RES", 2, '-', '-', '-', '-', 8, 0, "4", 1, "C", 1};
  case 0xa2:
    return {0xa2, "RES", 2, '-', '-', '-', '-', 8, 0, "4", 1, "D", 1};
  case 0xa3:
    return {0xa3, "RES", 2, '-', '-', '-', '-', 8, 0, "4", 1, "E", 1};
  case 0xa4:
    return {0xa4, "RES", 2, '-', '-', '-', '-', 8, 0, "4", 1, "H", 1};
  case 0xa5:
    return {0xa5, "RES", 2, '-', '-', '-', '-', 8, 0, "4", 1, "L", 1};
  case 0xa6:
    return {0xa6, "RES", 2, '-', '-', '-', '-', 16, 0, "4", 1, "HL", 1};
  case 0xa7:
    return {0xa7, "RES", 2, '-', '-', '-', '-', 8, 0, "4", 1, "A", 1};
  case 0xa8:
    return {0xa8, "RES", 2, '-', '-', '-', '-', 8, 0, "5", 1, "B", 1};
  case 0xa9:
    return {0xa9, "RES", 2, '-', '-', '-', '-', 8, 0, "5", 1, "C", 1};
  case 0xaa:
    return {0xaa, "RES", 2, '-', '-', '-', '-', 8, 0, "5", 1, "D", 1};
  case 0xab:
    return {0xab, "RES", 2, '-', '-', '-', '-', 8, 0, "5", 1, "E", 1};
  case 0xac:
    return {0xac, "RES", 2, '-', '-', '-', '-', 8, 0, "5", 1, "H", 1};
  case 0xad:
    return {0xad, "RES", 2, '-', '-', '-', '-', 8, 0, "5", 1, "L", 1};
  case 0xae:
    return {0xae, "RES", 2, '-', '-', '-', '-', 16, 0, "5", 1, "HL", 1};
  case 0xaf:
    return {0xaf, "RES", 2, '-', '-', '-', '-', 8, 0, "5", 1, "A", 1};
  case 0xb0:
    return {0xb0, "RES", 2, '-', '-', '-', '-', 8, 0, "6", 1, "B", 1};
  case 0xb1:
    return {0xb1, "RES", 2, '-', '-', '-', '-', 8, 0, "6", 1, "C", 1};
  case 0xb2:
    return {0xb2, "RES", 2, '-', '-', '-', '-', 8, 0, "6", 1, "D", 1};
  case 0xb3:
    return {0xb3, "RES", 2, '-', '-', '-', '-', 8, 0, "6", 1, "E", 1};
  case 0xb4:
    return {0xb4, "RES", 2, '-', '-', '-', '-', 8, 0, "6", 1, "H", 1};
  case 0xb5:
    return {0xb5, "RES", 2, '-', '-', '-', '-', 8, 0, "6", 1, "L", 1};
  case 0xb6:
    return {0xb6, "RES", 2, '-', '-', '-', '-', 16, 0, "6", 1, "HL", 1};
  case 0xb7:
    return {0xb7, "RES", 2, '-', '-', '-', '-', 8, 0, "6", 1, "A", 1};
  case 0xb8:
    return {0xb8, "RES", 2, '-', '-', '-', '-', 8, 0, "7", 1, "B", 1};
  case 0xb9:
    return {0xb9, "RES", 2, '-', '-', '-', '-', 8, 0, "7", 1, "C", 1};
  case 0xba:
    return {0xba, "RES", 2, '-', '-', '-', '-', 8, 0, "7", 1, "D", 1};
  case 0xbb:
    return {0xbb, "RES", 2, '-', '-', '-', '-', 8, 0, "7", 1, "E", 1};
  case 0xbc:
    return {0xbc, "RES", 2, '-', '-', '-', '-', 8, 0, "7", 1, "H", 1};
  case 0xbd:
    return {0xbd, "RES", 2, '-', '-', '-', '-', 8, 0, "7", 1, "L", 1};
  case 0xbe:
    return {0xbe, "RES", 2, '-', '-', '-', '-', 16, 0, "7", 1, "HL", 1};
  case 0xbf:
    return {0xbf, "RES", 2, '-', '-', '-', '-', 8, 0, "7", 1, "A", 1};
  case 0xc0:
    return {0xc0, "SET", 2, '-', '-', '-', '-', 8, 0, "0", 1, "B", 1};
  case 0xc1:
    return {0xc1, "SET", 2, '-', '-', '-', '-', 8, 0, "0", 1, "C", 1};
  case 0xc2:
    return {0xc2, "SET", 2, '-', '-', '-', '-', 8, 0, "0", 1, "D", 1};
  case 0xc3:
    return {0xc3, "SET", 2, '-', '-', '-', '-', 8, 0, "0", 1, "E", 1};
  case 0xc4:
    return {0xc4, "SET", 2, '-', '-', '-', '-', 8, 0, "0", 1, "H", 1};
  case 0xc5:
    return {0xc5, "SET", 2, '-', '-', '-', '-', 8, 0, "0", 1, "L", 1};
  case 0xc6:
    return {0xc6, "SET", 2, '-', '-', '-', '-', 16, 0, "0", 1, "HL", 1};
  case 0xc7:
    return {0xc7, "SET", 2, '-', '-', '-', '-', 8, 0, "0", 1, "A", 1};
  case 0xc8:
    return {0xc8, "SET", 2, '-', '-', '-', '-', 8, 0, "1", 1, "B", 1};
  case 0xc9:
    return {0xc9, "SET", 2, '-', '-', '-', '-', 8, 0, "1", 1, "C", 1};
  case 0xca:
    return {0xca, "SET", 2, '-', '-', '-', '-', 8, 0, "1", 1, "D", 1};
  case 0xcb:
    return {0xcb, "SET", 2, '-', '-', '-', '-', 8, 0, "1", 1, "E", 1};
  case 0xcc:
    return {0xcc, "SET", 2, '-', '-', '-', '-', 8, 0, "1", 1, "H", 1};
  case 0xcd:
    return {0xcd, "SET", 2, '-', '-', '-', '-', 8, 0, "1", 1, "L", 1};
  case 0xce:
    return {0xce, "SET", 2, '-', '-', '-', '-', 16, 0, "1", 1, "HL", 1};
  case 0xcf:
    return {0xcf, "SET", 2, '-', '-', '-', '-', 8, 0, "1", 1, "A", 1};
  case 0xd0:
    return {0xd0, "SET", 2, '-', '-', '-', '-', 8, 0, "2", 1, "B", 1};
  case 0xd1:
    return {0xd1, "SET", 2, '-', '-', '-', '-', 8, 0, "2", 1, "C", 1};
  case 0xd2:
    return {0xd2, "SET", 2, '-', '-', '-', '-', 8, 0, "2", 1, "D", 1};
  case 0xd3:
    return {0xd3, "SET", 2, '-', '-', '-', '-', 8, 0, "2", 1, "E", 1};
  case 0xd4:
    return {0xd4, "SET", 2, '-', '-', '-', '-', 8, 0, "2", 1, "H", 1};
  case 0xd5:
    return {0xd5, "SET", 2, '-', '-', '-', '-', 8, 0, "2", 1, "L", 1};
  case 0xd6:
    return {0xd6, "SET", 2, '-', '-', '-', '-', 16, 0, "2", 1, "HL", 1};
  case 0xd7:
    return {0xd7, "SET", 2, '-', '-', '-', '-', 8, 0, "2", 1, "A", 1};
  case 0xd8:
    return {0xd8, "SET", 2, '-', '-', '-', '-', 8, 0, "3", 1, "B", 1};
  case 0xd9:
    return {0xd9, "SET", 2, '-', '-', '-', '-', 8, 0, "3", 1, "C", 1};
  case 0xda:
    return {0xda, "SET", 2, '-', '-', '-', '-', 8, 0, "3", 1, "D", 1};
  case 0xdb:
    return {0xdb, "SET", 2, '-', '-', '-', '-', 8, 0, "3", 1, "E", 1};
  case 0xdc:
    return {0xdc, "SET", 2, '-', '-', '-', '-', 8, 0, "3", 1, "H", 1};
  case 0xdd:
    return {0xdd, "SET", 2, '-', '-', '-', '-', 8, 0, "3", 1, "L", 1};
  case 0xde:
    return {0xde, "SET", 2, '-', '-', '-', '-', 16, 0, "3", 1, "HL", 1};
  case 0xdf:
    return {0xdf, "SET", 2, '-', '-', '-', '-', 8, 0, "3", 1, "A", 1};
  case 0xe0:
    return {0xe0, "SET", 2, '-', '-', '-', '-', 8, 0, "4", 1, "B", 1};
  case 0xe1:
    return {0xe1, "SET", 2, '-', '-', '-', '-', 8, 0, "4", 1, "C", 1};
  case 0xe2:
    return {0xe2, "SET", 2, '-', '-', '-', '-', 8, 0, "4", 1, "D", 1};
  case 0xe3:
    return {0xe3, "SET", 2, '-', '-', '-', '-', 8, 0, "4", 1, "E", 1};
  case 0xe4:
    return {0xe4, "SET", 2, '-', '-', '-', '-', 8, 0, "4", 1, "H", 1};
  case 0xe5:
    return {0xe5, "SET", 2, '-', '-', '-', '-', 8, 0, "4", 1, "L", 1};
  case 0xe6:
    return {0xe6, "SET", 2, '-', '-', '-', '-', 16, 0, "4", 1, "HL", 1};
  case 0xe7:
    return {0xe7, "SET", 2, '-', '-', '-', '-', 8, 0, "4", 1, "A", 1};
  case 0xe8:
    return {0xe8, "SET", 2, '-', '-', '-', '-', 8, 0, "5", 1, "B", 1};
  case 0xe9:
    return {0xe9, "SET", 2, '-', '-', '-', '-', 8, 0, "5", 1, "C", 1};
  case 0xea:
    return {0xea, "SET", 2, '-', '-', '-', '-', 8, 0, "5", 1, "D", 1};
  case 0xeb:
    return {0xeb, "SET", 2, '-', '-', '-', '-', 8, 0, "5", 1, "E", 1};
  case 0xec:
    return {0xec, "SET", 2, '-', '-', '-', '-', 8, 0, "5", 1, "H", 1};
  case 0xed:
    return {0xed, "SET", 2, '-', '-', '-', '-', 8, 0, "5", 1, "L", 1};
  case 0xee:
    return {0xee, "SET", 2, '-', '-', '-', '-', 16, 0, "5", 1, "HL", 1};
  case 0xef:
    return {0xef, "SET", 2, '-', '-', '-', '-', 8, 0, "5", 1, "A", 1};
  case 0xf0:
    return {0xf0, "SET", 2, '-', '-', '-', '-', 8, 0, "6", 1, "B", 1};
  case 0xf1:
    return {0xf1, "SET", 2, '-', '-', '-', '-', 8, 0, "6", 1, "C", 1};
  case 0xf2:
    return {0xf2, "SET", 2, '-', '-', '-', '-', 8, 0, "6", 1, "D", 1};
  case 0xf3:
    return {0xf3, "SET", 2, '-', '-', '-', '-', 8, 0, "6", 1, "E", 1};
  case 0xf4:
    return {0xf4, "SET", 2, '-', '-', '-', '-', 8, 0, "6", 1, "H", 1};
  case 0xf5:
    return {0xf5, "SET", 2, '-', '-', '-', '-', 8, 0, "6", 1, "L", 1};
  case 0xf6:
    return {0xf6, "SET", 2, '-', '-', '-', '-', 16, 0, "6", 1, "HL", 1};
  case 0xf7:
    return {0xf7, "SET", 2, '-', '-', '-', '-', 8, 0, "6", 1, "A", 1};
  case 0xf8:
    return {0xf8, "SET", 2, '-', '-', '-', '-', 8, 0, "7", 1, "B", 1};
  case 0xf9:
    return {0xf9, "SET", 2, '-', '-', '-', '-', 8, 0, "7", 1, "C", 1};
  case 0xfa:
    return {0xfa, "SET", 2, '-', '-', '-', '-', 8, 0, "7", 1, "D", 1};
  case 0xfb:
    return {0xfb, "SET", 2, '-', '-', '-', '-', 8, 0, "7", 1, "E", 1};
  case 0xfc:
    return {0xfc, "SET", 2, '-', '-', '-', '-', 8, 0, "7", 1, "H", 1};
  case 0xfd:
    return {0xfd, "SET", 2, '-', '-', '-', '-', 8, 0, "7", 1, "L", 1};
  case 0xfe:
    return {0xfe, "SET", 2, '-', '-', '-', '-', 16, 0, "7", 1, "HL", 1};
  case 0xff:
    return {0xff, "SET", 2, '-', '-', '-', '-', 8, 0, "7", 1, "A", 1};
  }
}