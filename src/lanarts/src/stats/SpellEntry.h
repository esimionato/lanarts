/*
 * SpellEntry.h:
 *  Defines the data that goes into a spell
 */

#ifndef SPELLENTRY_H_
#define SPELLENTRY_H_

#include <string>
#include <vector>

#include "data/ResourceEntryBase.h"

#include <lcommon/LuaLazyValue.h>

#include "lanarts_defines.h"

#include "items/items.h"

struct SpellEntry: public ResourceEntryBase {
	sprite_id sprite;
	int mp_cost, cooldown;
	LuaLazyValue action_func; //Immediate action
	LuaLazyValue autotarget_func; //Auto-target func
	LuaLazyValue prereq_func; //Pre-req to casting
	Projectile projectile; //Projectile used, if any
	bool can_cast_with_cooldown, can_cast_with_held_key, fallback_to_melee;
	SpellEntry() :
			sprite(-1), mp_cost(0), cooldown(0), can_cast_with_cooldown(false), can_cast_with_held_key(
					false), fallback_to_melee(false) {
	}
	virtual const char* entry_type() {
		return "Spell";
	}
	virtual sprite_id get_sprite() {
		return sprite;
	}

	void init(lua_State* L) {
		action_func.initialize(L);
		autotarget_func.initialize(L);
		prereq_func.initialize(L);
	}
	bool uses_projectile() {
		return projectile.id != NO_ITEM;
	}
};

extern std::vector<SpellEntry> game_spell_data;

spell_id get_spell_by_name(const char* name);
#endif /* SPELLENTRY_H_ */
