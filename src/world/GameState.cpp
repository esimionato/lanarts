/*
 * GameState.cpp
 *
 *  Created on: 2011-09-29
 *      Author: 100397561
 */

#include "GameState.h"
#include "../display/display.h"
#include <SDL.h>
#include <cmath>
#include <SDL_opengl.h>
#include <cstring>

GameState::GameState(int width, int height, int vieww, int viewh, int hudw) :
	world_width(width), world_height(height),
	tiles(	width/TILE_SIZE, height/TILE_SIZE ),
	inst_set( width, height ),
	hud( vieww, 0, hudw, viewh ),
	view(50,50, vieww, viewh, width, height){
		memset(key_states, 0, sizeof(key_states));
		init_font(&pfont, "res/arial.ttf", 10);
		player = 0;
}

GameState::~GameState() {
	release_font(&pfont);
}



/*Handle new characters and exit signals*/
int GameState::handle_event(SDL_Event *event){
	int done;

	done = 0;
	switch (event->type) {
	case SDL_ACTIVEEVENT:
		break;

	case SDL_KEYDOWN: {
		if (event->key.keysym.sym == SDLK_r){

		}
		key_states[event->key.keysym.sym] = 1;
		break;
	}
	case SDL_KEYUP: {
		key_states[event->key.keysym.sym] = 0;
		break;
	}
	case SDL_MOUSEBUTTONUP: {
		left_click = true;
		break;
	}
	case SDL_QUIT:
		done = 1;
		break;
	}
	return (done);
}
bool GameState::step() {
	SDL_Event event;
	std::vector<GameInst*> safe_copy = inst_set.to_vector();
	//memset(key_states, 0, sizeof(key_states));
	left_click = false;
	SDL_GetMouseState( &mousex, &mousey );
	while( SDL_PollEvent( &event ) ) {
		if (handle_event(&event)) return false;
	}
	for (int i = 0; i < safe_copy.size(); i++){
		safe_copy[i]->step(this);
	}
	SDL_Delay(10);
	return true;
}

int GameState::key_press_state(int keyval){
	return key_states[keyval];
}
void GameState::draw() {
	int vp[4];

	gl_set_drawing_area(0,0,view.width,view.height);
	glClearColor( 0.0, 0.0, 0.0, 1.0 );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	tiles.draw(this);
	std::vector<GameInst*> safe_copy = inst_set.to_vector();
	for (size_t i = 0; i < safe_copy.size(); i++){
		safe_copy[i]->draw(this);
	}

	hud.draw(this);
	update_display();
	glFinish();
}

obj_id GameState::add_instance(GameInst *inst){
	return inst_set.add(inst);
}

void GameState::remove_instance(GameInst* inst){
	inst_set.remove(inst);
}

GameInst* GameState::get_instance(obj_id id){
	return inst_set.get_by_id(id);
}

static int squish(int a, int b, int c){
	return std::min(std::max(a,b), c-1);
}

static bool sqr_line_test(int x, int y, int w, int h, int sx, int sy, int size){

}
bool GameState::tile_line_test(int x, int y, int w, int h){
	int sx = x / TILE_SIZE, sy = y /TILE_SIZE;
}

static bool circle_line_test(int px, int py, int qx, int qy, int cx, int cy, float radsqr){
	int dx, dy, t, rt, ddist;
	dx = qx - px;
	dy = qy - py;
	ddist = dx*dx + dy*dy;
	t = -((px - cx)*dx + (py - cy)*dy);
			//;/ ddist;

	/* Restrict t to within the limits of the line segment */
	if(t < 0)  t = 0;  else if(t > ddist) t = ddist;

	dx = (px + t*(qx - px)/ddist) - cx;
	dy = (py + t*(qy - py)/ddist) - cy;
	rt = (dx*dx) + (dy*dy);
	return rt < (radsqr);
}
//game_id GameState::collides_with(){
//}

bool GameState::tile_radius_test(int x, int y, int rad){
	int w = world_width/TILE_SIZE, h = world_height/TILE_SIZE;
	//(rad*2) **2 area
	//should test x, y positions filling in circle
	int distsqr = (TILE_SIZE/2+rad), radsqr = rad*rad;
	distsqr *= distsqr;//sqr it

	int mingrid_x = (x-rad)/TILE_SIZE, mingrid_y = (y-rad)/TILE_SIZE;
	int maxgrid_x = (x+rad)/TILE_SIZE, maxgrid_y = (y+rad)/TILE_SIZE;
	int minx = squish(mingrid_x, 0, w), miny = squish(mingrid_y, 0, h);
	int maxx = squish(maxgrid_x, 0, w), maxy = squish(maxgrid_y, 0, h);

	//printf("minx=%d,miny=%d,maxx=%d,maxy=%d\n",minx,miny,maxx,maxy);

	for (int yy = miny; yy <= maxy; yy++){
		for (int xx = minx; xx <= maxx; xx++){
			if (tiles.get(xx,yy) > 0){
				int offset = TILE_SIZE/2;//To and from center
				int cx = int(xx*TILE_SIZE) + offset;
				int cy = int(yy*TILE_SIZE) + offset;
				int ydist = cy - y;
				int xdist = cx - x;
				double ddist = ydist*ydist + xdist*xdist;
				if ( ddist < distsqr ||
						circle_line_test(cx-offset, cy-offset, cx+offset,cy-offset,x,y,radsqr) ||
						circle_line_test(cx-offset, cy-offset, cx-offset,cy+offset,x,y,radsqr) ||
						circle_line_test(cx-offset, cy+offset, cx+offset,cy+offset,x,y,radsqr) ||
						circle_line_test(cx+offset, cy-offset, cx+offset,cy+offset,x,y,radsqr)){
					return true;
				}
			}
		}
		//printf("\n");
	}
	return false;
}
