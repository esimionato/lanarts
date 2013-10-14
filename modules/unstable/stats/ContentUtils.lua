local Display = import "core.Display"

local ObjectUtils = import "lanarts.objects.ObjectUtils"
local Apts = import "@stats.AptitudeTypes"
local SpellsKnown = import "@SpellsKnown"
local StatContext = import "@StatContext"
local SpellType = import "@SpellType"
local Stats = import "@Stats"
local SkillType = import "@SkillType"
local Attacks = import "@Attacks"

local M = nilprotect {} -- Submodule

M.RESOURCE_METATABLE = {
    __index = function (t,k) return assert(rawget(t, "type"))[k] end,
    __copy = function(t1,t2) table.copy(t1,t2, --[[Do not invoke meta]] false) end
}

-- Scan and call 'path_resolve' with call frame that begins with 'Define' prefix
function M.path_resolve_for_definition(path)
    for idx=1,math.huge do
        local info = debug.getinfo(idx)
        if not info then 
            return nil
        end
        local src = info.source
        local idx = src:find("/Define")
        if idx then 
            return src:sub(2,idx) .. path
        end
    end
end

function M.derive_on_draw(args)
    if type(args) == "function" then return args end
    if type(args) == "string" then args = M.derive_sprite(args) end
    if getmetatable(args) then -- Is it a plain table?
        args = { sprite = args }
    end

    return function(self, stats, drawf, options)
        if args.new_color then
            options.color = args.new_color
        end
        local function new_drawf(options)
            drawf(options)
            ObjectUtils.screen_draw(self.sprite, options.xy, args.alpha, args.frame, args.direction, args.color)
        end
        return new_drawf, options
    end
end

-- Derive sprite from name
function M.derive_sprite(name)
    local path = M.path_resolve_for_definition('sprites/')  .. name:gsub(' ', '_'):lower() .. ".png"
    return image_cached_load(path)
end

function M.resolve_sprite(args)
    local sprite = args.sprite or M.derive_sprite(args.lookup_key or args.name)
    if type(sprite) == "string" then
        if sprite:find("%(") or sprite:find("%%") then
            sprite = M.path_resolve_for_definition(sprite)
            return Display.directional_create(Display.images_load(sprite), 1.0)
        else
            return Display.image_load(sprite)
        end
    end
    return sprite
end

-- Resolves bonuses of the form [Apts.FOOBAR] = {0,1,1,0}
-- or [Apts.FOOBAR] = 2, equivalent to {2,2,2,2}
-- Expects a mutable 'table' that is being used to define a resource type
-- It will remove the 'convenience variables' used to define the aptitudes.
function M.resolve_aptitude_bonuses(table, --[[Optional]] result)
    result = result or {effectiveness={},damage={},resistance={},defence={}}
    for key,val in pairs(table) do
        if Apts.allowed_aptitudes[key] then
            local eff,dam,res,def
            if type(val) == 'number' then
                eff,dam,res,def = val,val,val,val
            else
                assert(#val == 4)
                eff,dam,res,def = unpack(val)
            end

            -- Add the bonus
            local curr_eff, curr_dam = result.effectiveness[key] or 0, result.damage[key] or 0 
            local curr_res, curr_def = result.resistance[key] or 0, result.defence[key] or 0
            result.effectiveness[key], result.damage[key] = curr_eff + eff, curr_dam + dam 
            result.resistance[key], result.defence[key] = curr_res + res, curr_def + def
 
            table[key] = nil -- Cleanup the aptitude convenience variable
        end
    end
    return result
end

function M.resolve_skills(table)
    local C = table.skill_costs or {}
    local skills = {}
    for k,v in pairs(C) do
        local type = assert(SkillType.lookup(k), k .. " is not a skill!")
        local skill = type:on_create()
        skill.cost_multiplier = skill.cost_multiplier * v
        _G.table.insert(skills, skill) 
    end
    table.skill_costs = nil
    return skills
end

-- Expects a mutable 'table' that is being used to define a resource type
-- It will remove the 'convenience variables' used to define the aptitudes.
function M.resolve_embedded_stats(table, --[[Optional]] resolve_skills)
    table.aptitudes = M.resolve_aptitude_bonuses(table)
    if resolve_skills then
        table.skills = M.resolve_skills(table)
    end
    local spells = table.spells
    if getmetatable(spells) ~= SpellsKnown then
        spells = SpellsKnown.create()
        for spell in values(table.spells or {}) do
            if type(spell) == "table" and not spell.id then
                local SpellDefineUtils = import "@spells.SpellDefineUtils"
                spell = SpellDefineUtils.spell_define(spell)
            end 
            spells:add_spell(spell)
        end
    end
    table.spells = spells
    return Stats.stats_create(table)
end

-- Returns an attack effect
function M.derive_attack_effect(args, --[[Optional, default false]] cleanup_member)
    local attack = args.unarmed_attack or args.attack
    local types = args.aptitude_types
    if not types and not attack then
        assert(not args.damage and not args.effectiveness, 
            "Incomplete attack specified, missing aptitude_types.") 
        return nil
    end

    -- First resolve 
    if attack and #attack > 0 then -- Resolve argument table
        attack = Attacks.attack_create(attack)
    end
    if not attack then
        attack = Attacks.attack_create(
            args.effectiveness or 0, args.damage or 0, types, 
            --[[Optional]] args.delay, --[[Optional]] args.damage_multiplier
        )
    end
    if cleanup_member then
        args.effectiveness, args.damage = nil
        args.attack, args.unarmed_attack, args.multipliers, args.type = nil
        args.delay, args.damage_multiplier = nil
    end
    return attack
end

function M.combine_on_prereq(p1, p2)
    return  --[[If]] p2 and --[[Then]] func_apply_and(p1, p2) or --[[Else]] p1
end

function M.combine_on_use(p1, p2)
    return  --[[If]] p2 and --[[Then]] func_apply_sequence(p1, p2) or --[[Else]] p1
end 

return M
