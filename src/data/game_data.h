/*
 * game_data.h
 *
 *  Created on: 2011-10-27
 *      Author: 100397561
 */

#ifndef GAME_DATA_H_
#define GAME_DATA_H_

#include "tile_data.h"
#include "sprite_data.h"


inline void init_game_data(){
	init_tile_data();
	init_sprite_data();
}



#endif /* GAME_DATA_H_ */
