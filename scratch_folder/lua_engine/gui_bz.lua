---@class gui
gui = {}

--- Draws text on the screen at the specified coordinates.
---@param x number
---@param y number
---@param message string
---@param color? string
function gui.text(x, y, message, color) end

--- Draws a pixel at the specified coordinates.
---@param x number
---@param y number
---@param color? string
function gui.pixel(x, y, color) end

--- Draws a line between two points.
---@param x1 number
---@param y1 number
---@param x2 number
---@param y2 number
---@param color? string
function gui.line(x1, y1, x2, y2, color) end

--- Draws a rectangle.
---@param x number
---@param y number
---@param width number
---@param height number
---@param fill? string
---@param outline? string
function gui.box(x, y, width, height, fill, outline) end
