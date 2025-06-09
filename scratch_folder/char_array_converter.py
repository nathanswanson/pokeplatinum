#! /usr/bin/env /home/wsl/pokeplatinum/.venv/bin/python3
# -*- coding: utf-8 -*-

import ast
import sys
from typing import Dict

CHAR_NULL = 0xFFFF

badges = [
    "COAL BADGE",
    "FORREST BADGE",
    "COBBLE BADGE",
    "BEACON BADGE",
    "ICICLE BADGE",
    "FEN BADGE",
    "RELIC BADGE",
    "MINE BADGE"
]

#grab all data from charmap.txt
char_data: Dict[str, int] = {}
with open("/home/wsl/pokeplatinum/tools/msgenc/charmap.txt") as char_file:
    for line in char_file.readlines():
        if "// Function codes" in line:
            break
        dat = line.split("=")
        if len(dat) != 2:
            continue
        char_data[dat[1][:-1]] = int(dat[0], 16)

if len(sys.argv) != 2:
    sys.stdout.write("Usage: char_array_converter.py <output_file>\n")
    sys.exit(1)
strings_in = ast.literal_eval(sys.argv[1])
if not isinstance(strings_in, list):
    strings_in = [strings_in]

for string in strings_in:
    sys.stdout.write(f"static const charcode_t {string.replace(' ', '_').lower()}_str[] = {{")

    for ch in string:
        if ch not in char_data:
            sys.stdout.write(hex(char_data["?"]))
        else:
            sys.stdout.write(hex(char_data[ch]))
        sys.stdout.write(", ")
    sys.stdout.write(f"{CHAR_NULL:#04x}")  # Append the null terminator
    sys.stdout.write("};\n")
sys.stdout.flush()
sys.exit(0)