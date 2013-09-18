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

#include <ldungeon_gen/Map.h>

#include "objects/TeamRelations.h"

#include "stats/EnemiesSeen.h"

#include "lanarts_defines.h"

#include "PlayerData.h"

class GameState;
class GameMapState;
class GeneratedRoom;
class PlayerInst;

class GameMaps {
public:
	std::vector<GameMapState*> maps;
};

class GameWorld {
public:
	GameWorld(GameState* gs);
	~GameWorld();
	void generate_room(GameMapState* level);
	GameMapState* get_level(int roomid, bool spawnplayer = false,
			void** player_instances = NULL, size_t nplayers = 0);
	bool pre_step();
	bool step();
	void level_move(int id, int x, int y, int roomid1, int roomid2);
	void set_current_level(int roomid);
	void set_current_level_lazy(int roomid);

	GameMapState* map_create(const Size& size, bool wandering_enabled = true);

	void reset(int keep = 0);
	void regen_level(int roomid);
	GameMapState* get_current_level() {
		return lvl;
	}
	void set_current_level(GameMapState* level) {
		lvl = level;
	}
	PlayerData& player_data() {
		return _player_data;
	}
	EnemiesSeen& enemies_seen() {
		return _enemies_seen;
	}

	int number_of_levels() {
		return level_states.size();
	}
	int get_current_level_id();
	void connect_entrance_to_exit(int roomid1, int roomid2);
	void serialize(SerializeBuffer& serializer);
	void deserialize(SerializeBuffer& serializer);

	void spawn_players(GameMapState* map, const std::vector<Pos>& positions);
private:
	void place_player(GameMapState* map, GameInst* p);
	void spawn_players(GeneratedRoom& genlevel, void** player_instances,
			size_t nplayers);
	bool midstep;
	int next_room_id;

	EnemiesSeen _enemies_seen;
	PlayerData _player_data;

	GameMapState* lvl;
	GameState* gs;
	std::vector<GameMapState*> level_states;
};

#endif /* GAMEWORLD_H_ */