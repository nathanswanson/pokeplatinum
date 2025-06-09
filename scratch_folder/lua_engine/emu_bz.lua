---@class emu
emu = {}

--- Advances the emulator by one frame.
function emu.frameadvance() end

--- Pauses the emulator.
function emu.pause() end

--- Returns the current frame count.
---@return integer
function emu.framecount()
    return 0
end

--- Returns the current system time in milliseconds.
---@return integer
function emu.time()
    return 0
end

--- Returns the current system frame rate.
---@return number
function emu.fps()
    return 0
end
