local COMMAND_SEND_ADDR = 0x2263290
local COMMAND_RESPONSE_ADDR = 0x2263298

local function read_val(pos)
    local cmd = 2
    cmd = cmd + bit.lshift(pos, 3)
    memory.writedword(COMMAND_SEND_ADDR, cmd)
    local response = memory.readdwordunsigned(COMMAND_RESPONSE_ADDR)
    return response
end

local function write_val(pos)
    local cmd = 1
    cmd = cmd + bit.lshift(pos, 3)
    memory.writedword(COMMAND_SEND_ADDR, cmd)
    print("Wrote command: " .. cmd)
    print(memory.readdword(COMMAND_SEND_ADDR))
end



local cooldown_time = 0.2 -- seconds
local last_press = {
    T = 0,
    Y = 0,
    U = 0,
    J = 0,
    G = 0,
    H = 0
}
local idx = 0

while true do
    local keys = input.read()
    local now = os.clock()
    if keys["T"] and now - last_press.T > cooldown_time then
        print(read_val(idx))
        last_press.T = now
    end
    if keys["Y"] and now - last_press.Y > cooldown_time then
        write_val(idx)
        last_press.Y = now
    end
    if keys["G"] and now - last_press.G > cooldown_time then
        memory.writedword(COMMAND_SEND_ADDR, 3)
        last_press.G = now
    end
    if keys["H"] and now - last_press.H > cooldown_time then
        print("sending item")
        local cmd = 1
        cmd = cmd + bit.lshift(idx, 4)
        cmd = cmd + bit.lshift(idx, 16)
        cmd = cmd + bit.lshift(1, 28)
        print("cmd: " .. cmd)
        memory.writedword(COMMAND_SEND_ADDR, cmd)
        last_press.H = now
    end
    if keys["U"] and now - last_press.U > cooldown_time then
        idx = idx + 1
        print(idx)
        last_press.U = now
    end
    if keys["J"] and now - last_press.J > cooldown_time then
        if idx > 0 then
            idx = idx - 1
        end
        print(idx)
        last_press.J = now
    end
    emu.frameadvance()
end
