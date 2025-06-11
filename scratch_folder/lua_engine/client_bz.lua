---@class client
client = {}

--- Logs a message to the console.
---@param message string
function client.log(message) end

--- Displays a message in a popup.
---@param message string
function client.message(message) end

--- Returns the current system time in milliseconds.
---@return integer
function client.getsystemtime()
    return 0
end

--- Returns the current system frame rate.
---@return number
function client.getfps()
    return 0
end

--- Returns the current frame count.
---@return integer
function client.getframes()
    return 0
end

--- Pauses the emulator.
function client.pause() end

--- Unpauses the emulator.
function client.unpause() end

--- Exits the emulator.
function client.exit() end

--- Returns the current state of the emulator (paused or running).
---@return boolean
function client.ispaused()
    return false
end

--- Returns the current screen dimensions.
---@return integer width, integer height
function client.getscreensize()
    return 0, 0
end

--- Reboots the currently loaded core
function client.reboot_core() end

--- Opens a ROM file.
--- @param path string
--- @return boolean success
function client.openrom(path)
    return false
end

--- Closes the currently loaded ROM.
function client.closerom() end

--- Opens Ram Watcher.
function client.openramwatch() end
