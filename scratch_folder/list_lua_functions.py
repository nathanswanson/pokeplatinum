import os
import re
# scrape
from bs4 import BeautifulSoup
import urllib.request as req
LUA_ENGINE_DIR = os.path.join(os.path.dirname(__file__), "lua_engine")

class_names = [

        "bit",
        "bizstring",
        "client",
        "comm",
        "console",
        "emu",
        "event",
        "forms",
        "gameinfo",
        "genesis",
        "gui",
        "input",
        "joypad",
        "LuaCanvas",
        "mainmemory",
        "memory",
        "memorysavestate",
        "movie",
        "nds",
        "nes",
        "savestate",
        "snes",
        "SQL",
        "tastudio",
        "userdata"
    ]

# grab any line that starts with "function " and captures the function name
function_pattern = re.compile(r'function\s+([a-zA-Z_][a-zA-Z0-9_.]*)\s*\(')

def find_lua_functions(folder):
    functions = []
    for filename in os.listdir(folder):
        if filename.endswith('.lua'):
            path = os.path.join(folder, filename)
            with open(path, 'r', encoding='utf-8') as f:
                content = f.read()
                found = function_pattern.findall(content)
                if found:
                    functions.extend(found)
    return functions

if __name__ == "__main__":
    funcs = find_lua_functions(LUA_ENGINE_DIR)
    for func in funcs:
        # print(f"{fname}:")
        print(f"{func}")

# scrape
full_list = []
soup = BeautifulSoup(req.urlopen("https://tasvideos.org/Bizhawk/LuaFunctions").read(), "html.parser")
for i, string in enumerate(soup.contents[2].contents[3].contents[5].contents[3].strings):
    # alternate color in console
    # if line contains ".", "()", and ")" print as function
    if string == "\n":
        continue
    if not ('.' in string or '()' in string or ')' in string):
        #print(f"\033[1;32m{string.strip()}\033[0m")
        continue
    if string.split(".")[0].strip() not in class_names:
        continue
    full_list.append(string.strip())

print("Functions:")
for func in full_list:
    if func not in funcs:
        # print in red
        print(f"\033[1;31m  {func}\033[0m")
    else:
        # print in green
        print(f"\033[1;32m  {func}\033[0m")
print(f"Total functions found: {len(full_list)}")
print(f"Total functions in lua_engine: {len(funcs)}")
print(f"Total functions missing: {len(full_list) - len(funcs)}")
print(f"Total classes: {len(class_names)}")
print(f"Total functions in classes: {len(full_list)}")
print(f"Total functions in lua_engine: {sum(len(v) for v in funcs.values())}")
print(f"Total functions in lua_engine (unique): {len(set(func for funcs in funcs.values() for func in funcs))}")
print(f"Total functions in lua_engine (unique, sorted): {len(set(sorted(func for funcs in funcs.values() for func in funcs)))}")