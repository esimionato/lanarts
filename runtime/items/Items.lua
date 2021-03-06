local GlobalData = require "core.GlobalData"
local EffectUtils = require "spells.EffectUtils"
local MiscSpellAndItemEffects = require "core.MiscSpellAndItemEffects"
local EventLog = require "ui.EventLog"

assert(GlobalData.keys_picked_up)

Data.item_create {
    name = "Gold", -- An entry named gold must exist, it is handled specially
    spr_item = "gold"
}

Data.item_create {
    name = "Azurite Key",
    type = "key",
    description = "Now that you have picked up this key, you can open Azurite doors.",
    use_message = "Now that you have picked up this key, you can open Azurite doors.",
    spr_item = "key1",
    pickup_func = function(self, user)
        if not GlobalData.keys_picked_up[self.name] then
            play_sound "sound/win sound 2-1.ogg"
        end
        GlobalData.keys_picked_up[self.name] = true 
    end,
    prereq_func = function (self, user)
        return false
    end,
    stackable = false,
    sellable = false
}

Data.item_create {
    name = "Dandelite Key",
    description = "Now that you have picked up this key, you can open Dandelite doors.",
    type = "key",
    use_message = "Now that you have picked up this key, you can open Dandelite doors.",
    spr_item = "key2",
    pickup_func = function(self, user)
        if not GlobalData.keys_picked_up[self.name] then
            play_sound "sound/win sound 2-1.ogg"
        end
        GlobalData.keys_picked_up[self.name] = true 
    end,
    prereq_func = function (self, user)
        return false
    end,
    sellable = false,
    stackable = false
}

Data.item_create {
    name = "Magentite Key",
    description = "Now that you have picked up this key, you can open Magentite doors.",
    type = "key",
    spr_item = "spr_keys.magentite_key",
    pickup_func = function(self, user)
        if not GlobalData.keys_picked_up[self.name] then
            play_sound "sound/win sound 2-1.ogg"
        end
        GlobalData.keys_picked_up[self.name] = true 
    end,
    prereq_func = function (self, user)
        return false
    end,
    sellable = false,
    stackable = false
}

Data.item_create {
    name = "Burgundite Key",
    description = "Now that you have picked up this key, you can open Burgundite doors.",
    type = "key",
    use_message = "Now that you have picked up this key, you can open Burgundite doors.",
    spr_item = "key3",
    pickup_func = function(self, user)
        if not GlobalData.keys_picked_up[self.name] then
            play_sound "sound/win sound 2-1.ogg"
        end
        GlobalData.keys_picked_up[self.name] = true 
    end,
    prereq_func = function (self, user)
        return false
    end,
    sellable = false,
    stackable = false
}

Data.item_create {
    name = "Mana Potion",
    description = "A magical potion of energizing infusions, it restores 50 MP to the user.",
    type = "potion",

    shop_cost = {15, 35},

    spr_item = "mana_potion",

    prereq_func = function (self, user)
        return user.stats.mp < user.stats.max_mp
    end,

    action_func = function(self, user)
        user:heal_mp(50)
    end
}

Data.item_create {
    name = "Red Mana Potion",
    description = "A magical potion of energizing infusions, it restores 25 MP to the user for every point of Red Power.",
    type = "potion",

    shop_cost = {15, 35},

    spr_item = "spr_scrolls.red_mana_potion",

    prereq_func = function (self, user)
        if EffectUtils.get_power(user, "Red") <= 0 then
            for _ in screens() do
                if caster:is_local_player() then
                    EventLog.add("You do not have any Red Power!", COL_PALE_RED)
                end
            end
            return false
        end
        return user.stats.mp < user.stats.max_mp 
    end,

    action_func = function(self, user)
        user:heal_mp(EffectUtils.get_power(user, "Red") * 25)
    end
}

Data.item_create {
    name = "Scroll of Fear",
    description = "Bestows the user with a terrible apparition, scaring away all enemies.",
    use_message = "You appear frightful!",

    shop_cost = {55,105},

    spr_item = "spr_scrolls.fear",

    action_func = function(self, user)
        user:add_effect("Fear Aura", 800).range = 120
    end
}

Data.item_create {
    name = "Scroll of Experience",
    description = "Bestows the user with a vision, leading to increased experience.",
    use_message = "Experience is bestowed upon you!",

    shop_cost = {55,105},

    spr_item = "scroll_exp",

    action_func = function(self, user)
        user:gain_xp(100)
    end
}

Data.item_create {
    name = "Defence Scroll",
    description = "A mantra of unnatural modification, it bestows the user with a permanent, albeit small, increase to defence.",
    use_message = "Defence is bestowed upon you!",

    shop_cost = {55,105},

    spr_item = "scroll_defence",

    action_func = function(self, user)
        user.stats.defence = user.stats.defence + 1
    end
}

Data.item_create {
    name = "Health Potion",
    description = "A blessed potion of healing infusions, it restores 50 HP to the user.",
    type = "potion",

    shop_cost = {15,35},

    spr_item = "health_potion",

    prereq_func = function (self, user) 
        return user.stats.hp < user.stats.max_hp 
    end,

    action_func = function(self, user)
        user:heal_hp(50)
    end
}

Data.item_create {
    name = "Strength Scroll",
    description = "A mantra of unnatural modification, it bestows the user with a permanent, albeit small, increase to strength.",
    use_message = "Strength is bestowed upon you!",

    shop_cost = {55,105},

    spr_item = "scroll_strength",

    action_func = function(self, user)
        user.stats.strength = user.stats.strength + 1
    end
}

Data.item_create  {
    name = "Haste Scroll",
    description = "A scroll that captures the power of the winds, it bestows the user with power and speed for a limited duration.",
    use_message = "You feel yourself moving faster and faster!",

    shop_cost = {25,45},

    spr_item = "scroll_haste",

    action_func = function(self, user)
        play_sound "sound/haste.ogg"
        user:add_effect("Haste", 800)
    end
}

Data.item_create  {
    name = "Will Scroll",
    description = "A mantra of unnatural modification, it bestows the user with a permanent, albeit small, increase to will.",
    use_message = "Will is bestowed upon you!",

    shop_cost = {55,105},

    spr_item = "scroll_will",

    action_func = function(self, user)
        user.stats.willpower = user.stats.willpower + 1
    end
}

Data.item_create {
    name = "Luminos Spellbook",
    use_message = "You learn Luminos!",
    description = "Teaches the spell 'Luminos'. The user must be a White Mage.",

    shop_cost = {55,205},

    spr_item = "spr_books.yellow",

    prereq_func = function(self, user)
        return user:has_effect "White Mage"
    end,
    action_func = function(self, user)
        user:learn_spell "Luminos"
    end
}

Data.item_create {
    name = "Fear Strike Manual",
    use_message = "You learn Fear Strike!",
    description = "Teaches the spell 'Fear Strike' to any class.",

    shop_cost = {55,105},

    spr_item = "spr_books.dark_gray",

    prereq_func = function(self, user)
        return true
    end,
    action_func = function(self, user)
        user:learn_spell "Fear Strike"
    end
}

Data.item_create {
    name = "Magic Scroll",
    use_message = "Magic is bestowed upon you!",
    description = "A mantra of unnatural modification, it bestows the user with a permanent, albeit small, increase to magic.",

    shop_cost = {55,105},

    spr_item = "scroll_magic",

    action_func = function(self, user)
        user.stats.magic = user.stats.magic + 1
    end
}

Data.item_create {
    name = "Magic Map",
    use_message = "You gain knowledge of your surroundings!",
    description = "A magic map that reveals the current level.",

    shop_cost = {25,35},

    spr_item = "spr_scrolls.magic-map",

    action_func = function(self, user)
        MiscSpellAndItemEffects.magic_map_effect(user)
        EventLog.add_all("The map is revealed!", {255,255,255})
    end
}

