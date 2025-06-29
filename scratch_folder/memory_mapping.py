import ast
import json
import os
import sys
desired_memory_positions = ["loc_data_registry", "ap_player_offset_table", "ap_player_names", "gStarterSpecies", "gBattleScriptSettings"]
data_offset = 0x1ffc000

if len(sys.argv) > 1:
    if sys.argv[1] == "debug":
        desired_memory_positions.append("desume_printf")

lines = []
start_found = False

with open(f"{os.path.dirname(__file__)}/../build/main.nef.xMAP") as f:
    for line in f:
        if line.startswith("# .main") or start_found:
            start_found = True
            lines.append(line)
# go to line "# .main"
# find ones with desired names
mappings = {}

for line in lines:
    for position_name in desired_memory_positions:
        if f" {position_name}\t" in line and ".text" not in line:
            offset = int(line.split()[0], 16) - data_offset
            size = int(line.split()[1], 16)
            mappings[position_name] = { "offset" : f"{hex(offset)}", "size": f"{hex(size)}" }
            desired_memory_positions.remove(position_name)
            break

if len(desired_memory_positions) != 0:
    print(f"values were not found: {desired_memory_positions}")
    # add missing values as -1
    for position_name in desired_memory_positions:
        mappings[position_name] = { "offset" : "-1", "size": "-1" }

# add the bus address
mappings["cmd_bus_addr"] = { "offset": "0x2263290", "size": "-1" }
mappings["recv_bus_addr"] = { "offset": "0x2263294", "size": "-1" }
json.dump(mappings, open(f"{os.path.dirname(__file__)}/out/memory_mapping.json", "w"), indent=4)
print(mappings)
