---@class memory
memory = {}

--- Reads a signed 8-bit integer from the specified address.
---@param addr integer
---@param domain? string
---@return integer
function memory.read_s8(addr, domain)
    return 0
end

--- Reads an unsigned 8-bit integer from the specified address.
---@param addr integer
---@param domain? string
---@return integer
function memory.read_u8(addr, domain)
    return 0
end

--- Reads a signed 16-bit little-endian integer from the specified address.
---@param addr integer
---@param domain? string
---@return integer
function memory.read_s16_le(addr, domain)
    return 0
end

--- Reads an unsigned 16-bit little-endian integer from the specified address.
---@param addr integer
---@param domain? string
---@return integer
function memory.read_u16_le(addr, domain)
    return 0
end

--- Reads a signed 32-bit little-endian integer from the specified address.
---@param addr integer
---@param domain? string
---@return integer
function memory.read_s32_le(addr, domain)
    return 0
end

--- Reads an unsigned 32-bit little-endian integer from the specified address.
---@param addr integer
---@param domain? string
---@return integer
function memory.read_u32_le(addr, domain)
    return 0
end

--- Writes an 8-bit integer to the specified address.
---@param addr integer
---@param value integer
---@param domain? string
function memory.write_u8(addr, value, domain) end

--- Writes a 16-bit little-endian integer to the specified address.
---@param addr integer
---@param value integer
---@param domain? string
function memory.write_u16_le(addr, value, domain) end

--- Writes a 32-bit little-endian integer to the specified address.
---@param addr integer
---@param value integer
---@param domain? string
function memory.write_u32_le(addr, value, domain) end
