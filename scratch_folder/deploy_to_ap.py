import shutil
import os
import bsdiff4
import hashlib as md
destination = "/mnt/c/Users/minne/OneDrive/Desktop/Archipelago/worlds/pokemon_platinum/data/"
source = "scratch_folder/out/"

original_rom_src = "/home/wsl/pokeplat_original/build/pokeplatinum.us.nds"
new_rom_src = "build/pokeplatinum.us.nds"

files = ["memory_mapping.json", "region_data.json", "platinum_base.bsdiff4"]

# first get bsdiff4 file
bsdiff4.file_diff(original_rom_src, new_rom_src, f"{source}/platinum_base.bsdiff4")
print(f"size of original rom: {os.path.getsize(original_rom_src)} bytes")
print(f"size of new rom: {os.path.getsize(new_rom_src)} bytes")
print(f"hash of original rom: {md.file_digest(open(original_rom_src, 'rb'), 'md5').hexdigest()}")
print(f"hash of new rom: {md.file_digest(open(new_rom_src, 'rb'), 'md5').hexdigest()}")
print(f"size of bsdiff4 file: {os.path.getsize(f'{source}/platinum_base.bsdiff4')} bytes")

shutil.copy(f"{source}/platinum_base.bsdiff4", destination)
print(f"Copied platinum_base.bsdiff4 to {destination}")
for file in files:
    shutil.copy(f"{source}/{file}", destination)
    print(f"Copied {file} to {destination}")