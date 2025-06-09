---
--- Created by wsl.
--- DateTime: 5/25/25 5:40 AM
---

--cmd 4 bits
--item_id 12 bits
--unused 12 bits
--player 4 bits

dofile("debug_message_listener.lua")
local COMMAND_SEND_ADDR = 0x2263290
local COMMAND_RESPONSE_ADDR = 0x2263298

COMMAND_NONE = 0
COMMAND_SEND_ITEM = 1
COMMAND_GET_ITEM_STATUS = 2


local function table_to_str(values)
    local ret_string = "["
    for i = 1, #values, 1 do
        ret_string = ret_string .. tostring(values[i])
        if i < #values then
            ret_string = ret_string .. ","
        end
    end
    ret_string = ret_string .. "]"
    return ret_string
end

function PLAYER_ID(id)
    return id << 28
end

function ITEM_ID(id)
    return id << 4 & 0xFFFFFF
end

function ReadClientResponse()
    return memory.read_u32_le(COMMAND_RESPONSE_ADDR)
end

function RomReady()
    local status = memory.read_u32_le(COMMAND_SEND_ADDR)
    return status == 0
end

function CommandAccepted()
    return memory.read_u32_le(COMMAND_SEND_ADDR) == 0xFFFF
end

function SendClientCommand(data)
    console.log("sending command")
    while not RomReady() do
        emu.frameadvance()
    end
    memory.write_u32_le(COMMAND_SEND_ADDR, data)
    while not CommandAccepted() do
        emu.frameadvance()
    end
end

function DisasmAtPC()
    return emu.disassemble(emu.getregister("ARM9 r15"))
end

function PopulateAPData()
    local data = CommData()
    local handle = io.popen(string.format("python3 memory_loader.py %s", table_to_str(data)))
    local result = handle:read("*a")
    console.log(result)
    local ret_code = handle:close()
end
