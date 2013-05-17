/* GameWorld:
 *  Contains player object information
 *  Contains enemy's seen information
 * 	Contains all the information and functionality of previously generated levels,
 * 	and which levels are currently being generated.
 */

#ifndef GAMEWORLD_H_
#define GAMEWORLD_H_

#include <vector>
#include <cstdlib>

#include "objects/TeamRelations.h"

#include "stats/EnemiesSeen.h"

#include "lanarts_defines.h"

#include "PlayerData.h"

class GameState;
class GameRoomState;
class GeneratedRoom;
class PlayerInst;

class GameWorld {
public:
	GameWorld(GameState* gs);
	~GameWorld();
	void generate_room(GameRoomState* level);
	GameRoomState* get_level(int roomid, bool spawnplayer = false,
			void** player_instances = NULL, size_t nplayers = 0);
	bool pre_step();
	bool step();
	void level_move(int id, int x, int y, int roomid1, int roomid2);
	void set_current_level(int roomid);
	void set_current_level_lazy(int roomid);
	void reset(int keep = 0);
	void regen_level(int roomid);
	void place_inst(GeneratedRoom& genlevel, GameInst* inst);
	GameRoomState* get_current_level() {
		return lvl;
	}
	void set_current_level(GameRoomState* level) {
		lvl = level;
	}
	PlayerData& player_data() {
		return _player_data;
	}
	EnemiesSeen& enemies_seen() {
		return _enemies_seen;
	}
	TeamRelations& teams() {
		return _teams;
	}
	int number_of_levels() {
		return level_states.size();
	}
	int get_current_level_id();
	void connect_entrance_to_exit(int roomid1, int roomid2);
	void serialize(SerializeBuffer& serializer);
	void deserialize(SerializeBuffer& serializer);
private:
	void spawn_players(GeneratedRoom& genlevel, void** player_instances,
			size_t nplayers);
	bool midstep;
	int next_room_id;

	EnemiesSeen _enemies_seen;
	PlayerData _player_data;
	TeamRelations _teams;

	GameRoomState* lvl;
	GameState* gs;
	std::vector<GameRoomState*> level_states;
};

#endif /* GAMEWORLD_H_ */