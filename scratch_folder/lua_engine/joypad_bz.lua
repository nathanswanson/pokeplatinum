---@class joypad
joypad = {}

--- Returns the current state of the joypad.
---@param controller? integer
---@return table<string, boolean>
function joypad.get(controller)
    return {}
end

--- Sets the state of the joypad.
---@param inputs table<string, boolean>
---@param controller? integer
function joypad.set(inputs, controller) end

--- Returns the immediate state of the joypad.
---@param controller? integer
---@return table<string, boolean>
function joypad.getimmediate(controller)
    return {}
end

--- Returns the state of the joypad, including buttons pressed by the current movie.
---@param controller? integer
---@return table<string, boolean>
function joypad.getwithmovie(controller)
    return {}
end

--- Sets the given analog controls to their provided values for the current frame.
--- Unlike set(), this only overrides with the given value.
---@param controls table<string, number>
---@param controller? integer
function joypad.setanalog(controls, controller) end

--- Sets the given buttons to their provided values for the current frame.
--- The string is interpreted as a movie input log entry.
---@param inputlogentry string
function joypad.setfrommnemonicstr(inputlogentry) end
