---@class console
console = {}

--- Clears the output box of the Lua Console window.
function console.clear() end

--- Returns a list of implemented functions.
---@return string
function console.getluafunctionslist()
    return ""
end

--- Outputs the given object to the output box on the Lua Console dialog. Can accept a LuaTable.
---@param outputs any
function console.log(outputs) end

--- Outputs the given object to the output box on the Lua Console dialog. Can accept a LuaTable.
---@param outputs any
function console.write(outputs) end

--- Outputs the given object to the output box on the Lua Console dialog. Can accept a LuaTable.
---@param outputs any
function console.writeline(outputs) end
