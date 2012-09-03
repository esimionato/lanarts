/*
 * projectile_data.h
 *  Represents spell/weapon/enemy's projectile data loaded from the yaml
 */

#ifndef PROJECTILE_DATA_H_
#define PROJECTILE_DATA_H_

#include <string>
#include <vector>

#include "../lua/LuaValue.h"

#include "../lanarts_defines.h"
#include "stats.h"

struct ProjectileEntry {
	std::string name, description;
	std::string weapon_class; //Compatible with this weapon class

	// Cost when appearing in shops (if an item), if (0,0) will not appear in shops.
	Range shop_cost;

	sprite_id item_sprite, attack_sprite;

	CoreStatMultiplier power, damage;
	float percentage_magic; //Conversely the rest is percentage physical
	float resist_modifier; // How much resistance can resist this attack, lower for fast attacks

	int drop_chance; //out of 100
	//If unarmed projectile, range/cooldown used
	//or if larger than base weapon's
	float speed;
	bool can_wall_bounce;
	int number_of_target_bounces;
	int cooldown, range, radius;

	LuaValue on_hit_func;

	ProjectileEntry() :
			item_sprite(-1), attack_sprite(-1), percentage_magic(0.0f), resist_modifier(
					1.0f), drop_chance(0), speed(0.0f), can_wall_bounce(false), number_of_target_bounces(
					0), cooldown(0), range(0), radius(0) {

	}

	bool is_unarmed() {
		return weapon_class == "unarmed" || weapon_class == "magic";
	}

	void init(lua_State* L) {
		on_hit_func.initialize(L);
	}
};

extern std::vector<ProjectileEntry> game_projectile_data;

projectile_id get_projectile_by_name(const char* name);

#endif /* PROJECTILE_DATA_H_ */
