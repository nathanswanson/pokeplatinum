---
--- Created by wsl.
--- DateTime: 5/25/25 2:28 PM
---
json = require("json")
function SWICallback()
    local str_ptr = emu.getregister("ARM9 r0") -- get the value of r0
    local s = ""
    local i = 0
    while true do
        local b = memory.read_u8(str_ptr + i)
        if b == 0 then break end
        s = s .. string.char(b)
        i = i + 1
    end
    console.log(string.format("[debug] %s", s))
end

function SWIAddress()
    local data = CommData()
    if data == nil then
        return -1
    end
    return data[1]
end

local function get_offset_addr(mappings, name)
    local hex_str = mappings[name]["offset"]
    hex_str = hex_str:gsub("^0x", "")
    return tonumber(hex_str, 16)
end

local function get_size_addr(mappings, name)
    local hex_str = mappings[name]["size"]
    hex_str = hex_str:gsub("^0x", "")
    return tonumber(hex_str, 16)
end

local comm_data_cache = nil

local function _comm_data()
    local mappings_file = io.open("out/memory_mapping.json")
    if mappings_file == nil then
        print("FILE NOT FOUND FOR MAPPINGS")
        return
    end
    local json_data = json.decode(mappings_file:read("*all"))
    local ret_list = {}
    table.insert(ret_list, get_offset_addr(json_data, "desume_printf") - 4)
    table.insert(ret_list, get_offset_addr(json_data, "loc_data_registry"))
    table.insert(ret_list, get_size_addr(json_data, "loc_data_registry"))
    table.insert(ret_list,get_offset_addr(json_data,"ap_player_names"))
    table.insert(ret_list,get_offset_addr(json_data,"ap_player_offset_table"))
    return ret_list
end

function CommData()
    if comm_data_cache == nil then
        comm_data_cache = _comm_data()
    end
    return comm_data_cache
end