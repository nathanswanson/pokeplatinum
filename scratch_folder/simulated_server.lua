---
--- Created by wsl.
--- DateTime: 5/23/25 1:30 PM
---
local Rx = require 'rx'

local COMMAND_SEND_ADDR = 0x2263290
local COMMAND_RESPONSE_ADDR = 0x2263298

local function read_val(pos, value)
    memory.writedword(COMMAND_SEND_ADDR, cmd)
end

Rx.Observable.fromRange(1, 8)
  :filter(function(x) return x % 2 == 0 end)
  :concat(Rx.Observable.of('who do we appreciate'))
  :map(function(value) return value .. '!' end)
  :subscribe(print)