/* GameWorld:
 * 	Contains all the information and functionality of previously generated levels,
 * 	and which levels are currently being generated
 */

#ifndef GAMEWORLD_H_
#define GAMEWORLD_H_
#include <vector>
#include <cstdlib>
#include "../util/game_basic_structs.h"

struct GameState;
struct GameLevelState;
struct GeneratedLevel;
struct PlayerInst;

class GameWorld {
public:
	GameWorld(GameState* gs, int width, int height);
	~GameWorld();
	void generate_room(GameLevelState* level);
	GameLevelState* get_level(int roomid, bool spawnplayer = false, void** player_instances = NULL, size_t nplayers = 0);
	void step();
	void level_move(int id, int x, int y, int roomid1, int roomid2);
	void set_current_level(int roomid);
	void set_current_level_lazy(int roomid);
	void reset(int keep = 0);
	void regen_level(int roomid);
	void spawn_player(GeneratedLevel& genlevel, bool local = true, PlayerInst* inst = NULL);
	GameLevelState*& get_current_level() {
		return lvl;
	}

	int get_current_level_id();
	void connect_entrance_to_exit(int roomid1, int roomid2);
private:
	bool midstep;
	int w, h;
	int next_room_id;
	GameLevelState* lvl;
	GameState* game_state;
	std::vector<GameLevelState*> level_states;
};



#endif /* GAMEWORLD_H_ */
