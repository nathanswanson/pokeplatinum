import ast
from dataclasses import dataclass
import sys
from io import BytesIO
from typing import Dict

import memory_mapping

#grab all data from charmap.txt
char_data: Dict[str, int] = {}
with open("../tools/msgenc/charmap.txt") as char_file:
    for line in char_file.readlines():
        if "// Function codes" in line:
            break
        dat = line.split("=")
        if len(dat) != 2:
            continue
        char_data[dat[1][:-1]] = int(dat[0], 16)

data_offset = 0x1ffc000


@dataclass
class Payload:
    is_local: bool
    id: int
    player_id: int

    def encode(self) -> int:
        short_val = self.is_local
        short_val |= self.id << 1
        short_val |= self.player_id << 13
        return short_val

addresses = ast.literal_eval(sys.argv[1])
print(f"addresses: {addresses}")
if addresses["loc_data_registry"]:
    print("writing ap location data")
    loc_data_offset = int(addresses["loc_data_registry"]["offset"],16) - data_offset
    loc_size = int(addresses["loc_data_registry"]["size"],16)
    int32_count = loc_size >> 1

    plat_path = "../build/pokeplatinum.us.nds"
    print(f"offset: {hex(loc_data_offset)}")
    # read entire file
    buffer: BytesIO
    with open(plat_path, "rb") as f:
        data = bytearray(f.read())
        buffer = BytesIO(data)

    # ap location data
    buffer.seek(loc_data_offset)
    #read each 32
    for i in range(int32_count):
        payload = Payload(True, i, 1)
        buffer.write(payload.encode().to_bytes(2, "little"))

# ap playernames
if addresses["ap_player_offset_table"] != -1 and addresses["ap_player_names"] != -1:
    print("Writing player name data/offsets")
    names = ["nathan", "Cnelly1022"]
    player_name_data_addr = int(addresses["ap_player_names"]["offset"], 16) - data_offset
    player_name_offsets_addr = int(addresses["ap_player_offset_table"]["offset"], 16) - data_offset
    # --playernames data
    buffer.seek(player_name_data_addr)
    cur_offset = 0
    offsets = []
    for name in names:
        offsets.append(cur_offset)
        for c in name:
            buffer.write(char_data[c].to_bytes(2, "little"))
            cur_offset += 1
        # null
        buffer.write(b"\x00\x00")
        cur_offset += 1

    buffer.seek(player_name_offsets_addr)
    for offst in offsets:
        buffer.write(offst.to_bytes(4, "little"))

# write changes
with open(plat_path, "wb") as f:
    f.write(buffer.getvalue())
