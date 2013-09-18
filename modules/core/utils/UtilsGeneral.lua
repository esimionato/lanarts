-- Utility code that can apply to lua programming in general.

--- Does nothing. 
--@usage dummy_object = { step = do_nothing, draw = do_nothing }
function do_nothing() end

--- Wraps a function around a memoizing weak table.
-- Function results will be stored until they are no longer referenced.
-- 
-- Note: This is intended for functions returning heavy-weight objects,
-- such as images. Functions that return primitives will not interact
-- correctly with the garbage collection.
--
-- @param func the function to memoize, arguments must be strings or numbers
-- @param separator <i>optional, default ';'</i>, the separator used when joining arguments to form a string key
-- @usage new_load = memoized(load)
function memoized(func, --[[Optional]] separator) 
    local cache = {}
    setmetatable( cache, {__mode = "kv"} ) -- Make table weak

    separator = separator or ";"

    return function(...)
        local key = table.concat({...}, separator)

        if not cache[key] then 
            cache[key] = func(...)
        end

        return cache[key]
    end
end

--- Return whether a file with the specified name exists.
-- More precisely, returns whether the given file can be opened for reading.
function file_exists(name)
    local f = io.open(name,"r")
    if f ~= nil then io.close(f) end
    return f ~= nil
end

function string:interpolate(table)
    return (self:gsub('($%b{})', function(w) return table[w:sub(3, -2)] or w end))
end

--- Get a  human-readable string from a lua value. The resulting value is generally valid lua.
-- Note that the paramaters should typically not used directly, except for perhaps 'packed'.
-- @param val the value to pretty-print
-- @param tabs <i>optional, default 0</i>, the level of indentation
-- @param packed <i>optional, default false</i>, if true, minimal spacing is used
-- @param quote_strings <i>optional, default true</i>, whether to print strings with spaces
function pretty_tostring(val, --[[Optional]] tabs, --[[Optional]] packed, --[[Optional]] quote_strings)
    tabs = tabs or 0
    quote_strings = (quote_strings == nil) or quote_strings

    local tabstr = ""

    if not packed then
        for i = 1, tabs do
            tabstr = tabstr .. "  "
        end
    end

    if type(val) == "string" and quote_strings then
        return tabstr .. "\"" .. val .. "\""
    end

    if type(val) ~= "table" then
        return tabstr .. tostring(val)
    end

    local parts = {"{", --[[sentinel for remove below]] ""}

    for k,v in pairs(val) do
        table.insert(parts, packed and "" or "\n") 

        if type(k) == "number" then
            table.insert(parts, pretty_tostring(v, tabs+1, packed))
        else 
            table.insert(parts, pretty_tostring(k, tabs+1, packed, false))
            table.insert(parts, " = ")
            table.insert(parts, pretty_tostring(v, type(v) == "table" and tabs+1 or 0, packed))
        end

        table.insert(parts, ", ")
    end

    parts[#parts] = nil -- remove comma or sentinel

    table.insert(parts, (packed and "" or "\n") .. tabstr .. "}");

    return table.concat(parts)
end

function table.clone(t)
    local newt = {}
    table.copy(t, newt)
    return newt
end

function table.key_list(t)
    local ret = {}
    for k,v in pairs(t) do
        table.insert(ret,k)
    end
    return ret
end

function table.deep_clone(t)
    local newt = {}
    table.deep_copy(t, newt)
    return newt
end

function table.pop_first(t)
    local len = #t
    if len == 0 then 
        return nil 
    end
    local val = t[len]
    for i=1,len-1 do
        t[i] = t[i+1]
    end
    t[len] = nil
    return val
end

function table.clear(t)
    for i=#t,1 do
        t[i] = nil
    end
    for k,v in pairs(t) do
        t[k] = nil
    end
end

function table.pop_last(t)
    local val = t[#t]
    t[#t] = nil
    return val    
end

function table.contains(t, val)
    for i=1,#t do 
        if t[i] == val then 
            return true
        end
    end
    return false
end

--- NOTE: Operates on array portion of table
-- Removes all occurences of 'val' from the table, compacting it
function table.remove_occurences(t, val)
    local new_len = 1
    -- Compact array
    for i = 1, #t do
        if t[i] ~= val then
            t[new_len] = t[i]
            new_len = new_len + 1
        end
    end
    for i = new_len, #t do
        t[i] = nil -- delete values at end
    end
end

function table.insert_all(t1, t2)
    for v in values(t2) do
        t1[#t1 + 1] = v
    end
end

--- Adds values from src to dest, copying them if initially nil
function table.defaulted_addition(src, dest)
    for k,v in pairs(src) do
        local val = dest[k]
        if dest[k] == nil then
            dest[k] = src[k]
        else
            if type(val) == 'table' then
                table.defaulted_addition(src[k], val)
            else
                dest[k] = val + src[k]
            end
        end
    end
end

--- Subtracts values from src to dest, copying them if initially nil
function table.defaulted_subtraction(src, dest)
    for k,v in pairs(src) do
        local val = dest[k]
        if dest[k] == nil then
            dest[k] = src[k]
        else
            if type(val) == 'table' then
                table.defaulted_subtraction(src[k], val)
            else
                dest[k] = val - src[k]
            end
        end
    end
end

function table.scaled(t, scale)
    local ret = {}
    for k,v in pairs(t) do
        ret[k] = v*scale
    end
    return ret
end

-- Resolves a number, or a random range
function random_resolve(v)
    return type(v) == "table" and random(unpack(v)) or v
end

--- Get a  human-readable string from a lua value. The resulting value is generally valid lua.
-- Note that the paramaters should typically not used directly, except for perhaps 'packed'.
-- @param val the value to pretty-print
-- @param tabs <i>optional, default 0</i>, the level of indentation
-- @param packed <i>optional, default false</i>, if true, minimal spacing is used
function pretty_print(val, --[[Optional]] tabs, --[[Optional]] packed)
    print(pretty_tostring(val, tabs, packed))
end

--- Iterate all iterators one after another
function iter_combine(...)
    local args = {...}
    local arg_n = #args
    local arg_i = 1
    local iter = args[arg_i]
    return function()
        while true do
            if not iter then return nil end
            local val = iter()
            if val ~= nil then return val end
            arg_i = arg_i + 1
            iter = args[arg_i]
        end
    end
end

--- Like a functional map of a function onto a list
function map_call(f, list)
    local ret = {}
    for v in values(list) do 
        ret[#ret + 1] = f(v)
    end
    return ret
end

-- Functional composition
function func_apply_and(f1,f2, --[[Optional]] s2)
    return function(s1,...) 
        local v = {f1(s1,...)}
        if not v[1] then return unpack(v)
        else 
            if s2 then return f2(s2, ...)
            else return f2(s1,...) end
        end
    end
end

function func_apply_not(f)
    return function(...) return not f(...) end
end

function func_apply_or(f1,f2, --[[Optional]] s2)
    return function(s1,...) 
        local v = {f1(s1,...)}
        if v[1] then return unpack(v)
        else 
            if s2 then return f2(s2, ...)
            else return f2(s1,...) end
        end
    end
end

function func_apply_sequence(f1,f2,--[[Optional]] s2)
    return function(s1,...)
        local v = f1(s1,...)
        if s2 then v = f2(s2, ...) or v 
        else v = f1(s1,...) or v end
        return v 
    end
end

local _cached_dup_table = {}
function dup(val, times)
    table.clear(_cached_dup_table)
    for i=1,times do
        _cached_dup_table[i] = val
    end
    return unpack(_cached_dup_table)
end

--- Return a random element from a list
function random_choice(choices)
    local idx = random(1, #choices)
    return choices[idx]
end