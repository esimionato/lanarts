/*
 * enemy_data.cpp
 *
 *  Created on: Mar 20, 2012
 *      Author: 100397561
 */

#ifndef ENEMY_DATA_CPP_
#define ENEMY_DATA_CPP_

#include "enemy_data.h"
#include "sprite_data.h"

//Stats constructor:
//	Stats(movespeed, hp, mp, can_melee, can_range, melee_dmg, range_dmg,
//			cooldown, mreach, range, bulletspeed = 7)
//EnemyType constructor:
//   EnemyType(name, rad, xpaward, spriten, stats)
Attack weakmelee(true,  10, 25, 70);
Attack medmelee(true,  15, 25, 70);
Attack strongmelee(true,  20, 25, 70);
Attack strongermelee(true,  45, 25, 70);
Attack strongshortrange_storm(true, 13, 130, 45, SPR_STORMBOLT, 7 );
Attack strongshortrange_fire(true, 30, 400, 70, SPR_FIREBOLT, 4 );

EnemyType game_enemy_data[] = {

		EnemyType("Skeleton", 11, 10, SPR_SKELETON,
				Stats(2, 30,0,
						Attack(true,  12, 25, 70),
						Attack()
				)
		),
		EnemyType("Chicken", 10, 10, SPR_CHICKEN,
				Stats(4, 10,0, Attack(true,  2, 25, 5), Attack())),
		EnemyType("Zombie", 14, 15, SPR_ZOMBIE,
				Stats(2, 40,0, medmelee, Attack())),
		EnemyType("Storm Elemental", 14, 20, SPR_STORM_ELE,
				Stats(1, 40,0, Attack(), strongshortrange_storm)),

		EnemyType("Ciribot", 14, 30, SPR_CIRIBOT,
				Stats(3, 60,0, Attack(true,  10, 45, 35), Attack())),


		EnemyType("Super Chicken", 10, 20, SPR_SUPERCHICKEN,
				Stats(4, 45,0, strongmelee, Attack())),
		EnemyType("Grue", 14, 35, SPR_GRUE,
				Stats(2, 100,0, strongermelee, Attack())),
		EnemyType("Jester", 14, 35, SPR_JESTER,
				Stats(5, 80,0, strongmelee, Attack())),
		EnemyType("Hell Forged", 14, 50, SPR_HELLFORGED,
				Stats(3, 120,0, Attack(), strongshortrange_fire)),
};


size_t game_enemy_n = sizeof(game_enemy_data)/sizeof(EnemyType);



#endif /* ENEMY_DATA_CPP_ */
