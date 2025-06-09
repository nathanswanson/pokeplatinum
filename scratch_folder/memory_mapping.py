import ast
import json

desired_memory_positions = ["loc_data_registry", "desume_printf", "ap_player_offset_table", "ap_player_names"]

lines = []
start_found = False
with open("../build/main.nef.xMAP") as f:
    for line in f:
        if line.startswith("# .main") or start_found:
            start_found = True
            lines.append(line)
        if "SDK_STATIC_BSS_END" in line:
            break
# go to line "# .main"
# find ones with desired names
mappings = {}

for line in lines:
    for position_name in desired_memory_positions:
        if f" {position_name}\t" in line:
            mappings[position_name] = { "offset" : f"0x{line.split()[0].strip()}", "size": f"0x{line.split()[1].strip()}" }
            desired_memory_positions.remove(position_name)
            break

if len(desired_memory_positions) != 0:
    print(f"values were not found: {desired_memory_positions}")
    # add missing values as -1
    for position_name in desired_memory_positions:
        mappings[position_name] = { "offset" : "-1", "size": "-1" }
json.dump(mappings, open("out/memory_mapping.json", "w"), indent=4)
print(mappings)