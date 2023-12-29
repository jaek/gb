#!/bin/python3
import json
import sys


# https://gbdev.io/gb-opcodes/Opcodes.json
opcodes_file = 'opcodes.json'
output_file = 'out.cpp'

def get_op(name, instruction):
  t = get_operand_type(name, instruction)
  if t == "flag" and name == "C":
    name = "CF"
  if t in ["a16", "a8", "n16", "n8", "s8", "none"]:
    name = "rom"
  if t == "vector":
    name = 'v' + name[1:]
  if t == "bit":
    name = 'b' + name
  return (name, t)

def get_operand_type(name, instruction):
  if instruction in ["JR", "CALL", "JP", "RET"] and name == "C":
    return "flag"
  if name in ["NZ", "NC", "Z"]:
    return "flag"
  v = ["$00", "$08", "$10", "$18", "$20", "$28", "$30", "$38"] 
  if name == "none":
    return "none"
  if name in v:
    return "vector"
  if name.isdigit():
    return "bit"
  if name in ["AF", "BC", "DE", "HL", "NC", "SP"]:
    return "reg16"
  if name in ["a16", "a8", "n16", "n8"]:
    return name
  if name == "e8":
    return "s8"
  return "reg8"

with open('opcodes.json', 'r') as f:
    js = json.load(f)

if output_file:
  f = open(output_file, 'w') 
  sys.stdout = f

i = 0
s = ''
ops = []
opl = list()
for p in ["unprefixed", "cbprefixed"]:
  if p[0] == 'c':
    print('OpCode get_op(uint8_t oc){')
  else: 
    print('OpCode get_cb(uint8_t oc){')
  print('switch(oc){')
  for opcode in js[p].keys():
      d = js[p][opcode]
      mnemonic = d["mnemonic"]
      length = d["bytes"]
      operands = d["operands"]
      d_suc = d["cycles"][0]
      d_fai = d["cycles"][1] if len(d["cycles"])>1 else 0
      flags = d["flags"]
      z = flags["Z"]
      n = flags["N"]
      h = flags["H"]
      c = flags["C"]

      name = operands[0]["name"] if len(operands) >= 1 else "none"
      o1, o1_t = get_op(name, mnemonic)
      name = operands[1]["name"] if len(operands) > 1 else "none"
      o2, o2_t = get_op(name, mnemonic)
      #opl = opl + [o1] + [o2]

      print(f'case {opcode.lower()}:')
      print('return {'
        + f'{opcode.lower()},"{mnemonic}",{length},'
        + f"'{z}','{n}','{h}','{c}',"
        + f'{d_suc},{d_fai},{o1_t},{o1},{o2_t},{o2}'
        + '};'
      )
  print('}')
  print('}')

# opl = list(set(opl))
# opl.sort()
# [print(o) for o in opl]

if output_file:
  f.close()