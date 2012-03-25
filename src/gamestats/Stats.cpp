/*
 * Stats.cpp
 *
 *  Created on: Mar 24, 2012
 *      Author: 100397561
 */

#include "Stats.h"

Stats::Stats(float speed, int hp, int mp, const Attack & melee,
		const Attack & ranged) :
		movespeed(speed), hp(hp), max_hp(hp), mp(mp), max_mp(mp), hpregen(
				1.0 / 30), mpregen(1.0 / 15), cooldown(0), hurt_cooldown(0), hp_regened(
				0), mp_regened(0), melee(melee), ranged(ranged), xp(0), xpneeded(
				100), xplevel(1) {
}

void Stats::step() {
	cooldown--;
	if (cooldown < 0)
		cooldown = 0;

	hurt_cooldown--;
	if (hurt_cooldown < 0)
		hurt_cooldown = 0;

	raise_hp(hpregen);
	raise_mp(mpregen);
}

void Stats::raise_hp(float hpgain) {
	hp_regened += hpgain;
	if (hp_regened > 0) {
		hp += floor(hp_regened);
		hp_regened -= floor(hp_regened);
	}
	if (hp > max_hp)
		hp = max_hp;

}

void Stats::raise_mp(float mpgain) {
	mp_regened += mpgain;
	if (mp_regened > 0) {
		mp += floor(mp_regened);
		mp_regened -= floor(mp_regened);
	}
	if (mp > max_mp)
		mp = max_mp;

}

bool Stats::has_cooldown() {
	return cooldown > 0;
}

float Stats::hurt_alpha() {
	if (hurt_cooldown < 10)
		return hurt_cooldown / 10 * 0.7 + 0.3;

	else
		return (20 - hurt_cooldown) / 10 * 0.7 + 0.3;

}

void Stats::set_hurt_cooldown() {
	if (hurt_cooldown == 0)
		hurt_cooldown = 20;

}

void Stats::reset_melee_cooldown() {
	cooldown = melee.cooldown;
}

void Stats::reset_ranged_cooldown() {
	cooldown = ranged.cooldown;
}

void Stats::reset_melee_cooldown(const Stats & effectivestats) {
	cooldown = effectivestats.melee.cooldown;
}

void Stats::reset_ranged_cooldown(const Stats & effectivestats) {
	cooldown = effectivestats.ranged.cooldown;
}

bool Stats::hurt(int dmg) {
	hp -= dmg;
	set_hurt_cooldown();
	if (hp < 0) {
		hp = 0;
		return true;
	}
	return false;
}

void Stats::gain_level() {
	hp += 10;
	max_hp += 10;
	mp += 10;
	max_mp += 10;
	melee.damage += 1;
	ranged.damage += 1;
	xplevel++;
}

void Stats::gain_xp(int amnt) {
	xp += amnt;
	if (xp >= xpneeded) {
		xp -= xpneeded;
		xpneeded = (xplevel - 1) * 100 + 50;
		gain_level();
	}
}