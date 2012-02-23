/*
 * GameState.h
 *
 *  Created on: 2011-09-29
 *      Author: 100397561
 */

#ifndef GAMESTATE_H_
#define GAMESTATE_H_
#include <vector>
#include <SDL.h>
#include "objects/GameInst.h"
#include "controllers/MonsterController.h"
#include "controllers/PlayerController.h"
#include "GameInstSet.h"
#include "GameTiles.h"
#include "GameView.h"
#include "GameHud.h"
#include "../util/font_util.h"
#include "../procedural/mtwist.h"
#include "../fov/fov.h"

class GameState {
public:

	GameState(int width, int height, int vieww = 640, int viewh = 480,
			int hudw = 160);
	~GameState();

	/* Each frame is divided into a step event, and a draw event */
	void draw();
	bool step();

	/* INSTANCE HANDLING METHODS */
	GameInst* get_instance(obj_id id);
	obj_id add_instance(GameInst* inst);
	void remove_instance(GameInst* inst);

	/* COLLISION METHODS */
	bool tile_radius_test(int x, int y, int rad);
	/* Check whether intersects a solid object */
	bool solid_radius_test(int x, int y, int rad);
	bool tile_line_test(int x, int y, int w, int h);
	int object_radius_test(GameInst* obj, GameInst** objs = NULL,
			int obj_cap = 0, col_filter f = NULL, int x = -1,
			int y = -1, int radius = -1);
	bool object_visible_test(GameInst* obj);

	/* GameState components */
	GameView& window_view() {
		return view;
	}
	GameTiles& tile_grid() {
		return tiles;
	}
	/* Game object central controllers */
	MonsterController& monster_controller() {
		return mc;
	}
	PlayerController& player_controller() {
		return pc;
	}

	/* Default font for most text rendering */
	const font_data& primary_font() {
		return pfont;
	}

	/* Mouse state information */
	int mouse_x() {
		return mousex;
	}
	int mouse_y() {
		return mousey;
	}
	bool mouse_left_click() {
		return left_click;
	}
	bool mouse_right_click() {
		return right_click;
	}

	/* Key state query information */
	int key_press_state(int keyval);

	/* Object identifier for the player */
	obj_id& player_id() {
		return player;
	}
	GameInst* player_obj() {
		return get_instance(player);
	}
	/* Field-of-view for the player */
	fov& player_fov() {
		return pfov;
	}

	/* Getters for world size */
	int width() {
		return world_width;
	}
	int height() {
		return world_height;
	}

	void serialize(FILE* file);

	MTwist& rng() {
		return mtwist;
	}

private:
	void restart();
	int world_width, world_height;
	int handle_event(SDL_Event* event);
	int frame_n;
	obj_id player;
	fov pfov;

	//Game world components
	GameTiles tiles;
	GameInstSet inst_set;
	GameHud hud;
	GameView view;

	//Game controllers
	MonsterController mc;
	PlayerController pc;

	//Mersenne twister random number generator state
	MTwist mtwist;

	//Primary font data
	font_data pfont;

	char key_states[SDLK_LAST];
	int mousex, mousey;
	bool left_click, right_click;
};

#endif /* GAMESTATE_H_ */
