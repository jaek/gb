#!/bin/python3
import json

# https://gbdev.io/gb-opcodes/Opcodes.json
with open('opcodes.json', 'r') as f:
    js = json.load(f)

i = 0
s = ''
for p in ["cbprefixed", "unprefixed"]:
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
      
      o1 = operands[0]["name"] if len(operands) >= 1 else ""
      if o1 != "":
        o1b = operands[0]["bytes"] if "bytes" in operands[0].keys() else "1"
      else:
        o1b = "0"
      o2 = operands[1]["name"] if len(operands) > 1 else ""
      if o2 != "":
        o2b = operands[1]["bytes"] if "bytes" in operands[1].keys() else "1"
      else:
        o2b = "0"
      print(f'{p}[{opcode.lower()}]=' + '{' + f'true,"{mnemonic}",{length},' + f"'{z}','{n}','{h}','{c}'," + f'{d_suc},{d_fai},"{o1}",{o1b},"{o2}",{o2b}'+'};')

