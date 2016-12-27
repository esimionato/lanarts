Data.equipment_create {
    name = "Belt of Protection",
    description = "A belt with a protective aura.",
    type = "belt",

    shop_cost = { 120, 140 },

    spr_item = "spr_belts.protection",

    reduction = { base = 1 },
    resistance = { base = 1 },
    magic_reduction = { base = 1 },
    magic_resistance = { base = 1 }
}

Data.equipment_create {
    name = "Belt of Slaying",
    description = "A belt infused with ogre strength.",
    type = "belt",

    shop_cost = { 120, 140 },

    spr_item = "spr_belts.slaying",

    stat_bonuses = {strength = 2}
}

Data.equipment_create {
    name = "Warped Belt",
    description = "A belt that decreases your HP and MP reserves significantly, but provides significant magical power.",
    type = "belt",

    shop_cost = { 200, 250 },

    spr_item = "spr_belts.warped",

    stat_bonuses = {magic = 5, mp = -40, hp = -50}
}



