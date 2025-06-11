---
--- Created by wsl.
--- DateTime: 5/23/25 1:30 PM
---
console.clear()
dofile("debug_message_listener.lua")
dofile("comms.lua")

local function sleep_script(frame_ct)
    for i = 1, frame_ct do
        emu.frameadvance()
    end
end

local function toggle_button(button, enabled)
    local current_input = joypad.get()
    current_input[button] = enabled
    joypad.set(current_input)
end

local function press_button(button)
    toggle_button(button, true)
end

local function set_touch_pos(x, y)
    local current_input = joypad.get()
    local analog_set = {}
    analog_set["Touch X"] = x
    analog_set["Touch Y"] = y
    current_input["Touch"] = true

    joypad.set(current_input)
    joypad.setanalog(analog_set)
end

local function hold_touch(x, y, ms)
    for i = 1, ms do
        if i == 1 then
            local analog_set = {}
            analog_set["Touch X"] = x
            analog_set["Touch Y"] = y
            joypad.setanalog(analog_set)
            emu.frameadvance()
        else
            set_touch_pos(x, y)
            emu.frameadvance()
        end
    end
    joypad.setanalog({})
    emu.frameadvance()
end

local function hold_button(button, ms)
    for i = 1, ms do
        press_button(button)
        emu.frameadvance()
    end
    emu.frameadvance()
end

local new_sha1 = gameinfo.getromhash()
local hash_file = io.open("out/platinum.sha1", "r")
local old_sha1
if hash_file == nil then
    old_sha1 = ""
else
    old_sha1 = hash_file:read("*all")
    hash_file:close()
end
console.log("Init Memory Manager")
event.on_bus_exec(SWICallback(), SWIAddress())
if old_sha1 ~= new_sha1 then
    console.log("hash changed rebuilding state")
    --launch game
    client.openrom("../build/pokeplatinum.us.nds")
    client.speedmode(6400)
    sleep_script(600)
    console.log("done waiting...")
    hold_button("A", 100)
    sleep_script(50)
    hold_button("A", 100)
    sleep_script(250)
    hold_button("A", 100)
    sleep_script(10)
    hold_button("A", 100)
    sleep_script(10)
    hold_button("A", 100)
    sleep_script(150)
    hold_button("A", 100)
    sleep_script(10)
    hold_button("A", 100)
    sleep_script(10)
    hold_button("A", 100)
    sleep_script(10)
    hold_button("A", 100)
    sleep_script(50)
    hold_button("Down", 100)
    sleep_script(10)
    hold_button("A", 10)
    sleep_script(350)
    hold_button("A", 10)
    sleep_script(100)
    hold_button("A", 10)
    sleep_script(50)
    hold_button("A", 10)
    sleep_script(300)
    console.log("start touch tutorial")
    hold_touch(130, 100, 300)
    sleep_script(100)
    hold_button("A", 100)
    sleep_script(10)
    hold_button("A", 100)
    sleep_script(10)
    hold_button("A", 100)
    sleep_script(10)
    hold_button("A", 100)
    sleep_script(10)
    hold_button("A", 100)
    sleep_script(10)
    hold_button("A", 100)
    sleep_script(20)
    hold_button("A", 100)
    sleep_script(50)
    hold_button("A", 100)
    sleep_script(200)
    console.log("gender/name/rival start")
    hold_button("A", 100)
    sleep_script(250)
    hold_button("A", 100)
    sleep_script(100)
    hold_button("A", 100)
    sleep_script(100)
    hold_button("A", 100)
    sleep_script(100)
    hold_button("A", 100)
    sleep_script(100)
    --name
    hold_button("A", 100)
    sleep_script(100)
    hold_button("Start", 50)
    sleep_script(50)
    hold_button("A", 50)
    sleep_script(200)
    hold_button("A", 50)
    sleep_script(100)
    hold_button("A", 100)
    sleep_script(100)
    hold_button("A", 100)
    sleep_script(100)
    hold_button("A", 100)
    sleep_script(100)
    hold_button("A", 100)
    sleep_script(100)
    hold_button("A", 100)
    sleep_script(100)
    hold_button("Down", 100)
    sleep_script(50)
    hold_button("A", 100)
    sleep_script(100)
    hold_button("A", 100)
    sleep_script(100)
    hold_button("A", 100)
    sleep_script(150)
    hold_button("A", 100)
    sleep_script(150)
    hold_button("A", 100)
    sleep_script(150)
    hold_button("A", 100)
    sleep_script(100)
    hold_button("A", 100)
    sleep_script(100)
    hold_button("A", 100)
    sleep_script(100)
    hold_button("A", 100)
    sleep_script(100)
    hold_button("A", 100)
    sleep_script(1000)
    hold_button("A", 100)
    sleep_script(500)
    hold_button("A", 100)
    sleep_script(100)
    hold_button("A", 100)
    sleep_script(100)
    hold_button("A", 100)
    sleep_script(100)
    hold_button("A", 100)
    sleep_script(100)
    hold_button("A", 100)
    console.log("Finished State Generation")
    console.log("Saving State")
    savestate.saveslot(1)
    console.log("Clossing ROM")
    client.closerom()

    console.log("Opening ROM")
    client.openrom("../build/pokeplatinum.us.nds")
    console.log("Writing new sha1")
    hash_file = io.open("out/platinum.sha1", "w+")
    if hash_file ~= nil then
        hash_file:write(gameinfo.getromhash())
        console.log("Clossing rom/file")
        hash_file:close()
    end
    client.closerom()
end
--complete tutorial

local function input_callback()
    local inputs = input.get()
    if inputs["T"] then
        if inputs["Shift"] then
            client.pause()
        end
        SendClientCommand(COMMAND_SEND_ITEM | PLAYER_ID(1) | ITEM_ID(470))
    end
end


console.log("Start ROM")
client.openrom("../build/pokeplatinum.us.nds")
client.openramwatch()
console.log("Event Register")
event.on_bus_exec(SWICallback, SWIAddress())
client.speedmode(6400)
savestate.loadslot(1)

console.log("Setup Complete starting tests.")
for i = 1, 100 do
    emu.frameadvance()
end



console.log("Ready")

event.oninputpoll(input_callback)
SendClientCommand(COMMAND_SEND_ITEM | PLAYER_ID(1) | ITEM_ID(5))
SendClientCommand(COMMAND_SEND_ITEM | PLAYER_ID(0) | ITEM_ID(470))
