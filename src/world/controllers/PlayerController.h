/*
 * PlayerController.h
 *	Handles network communication of player coordinates, creates input actions for player objects.
 *  TODO: Implement action system, currently players directly respond to keyboard commands.
 */

#ifndef PLAYERCONTROLLER_H_
#define PLAYERCONTROLLER_H_
#include <vector>
#include "../objects/GameInst.h"
#include "../../fov/fov.h"

class PlayerController {
public:
	PlayerController();
	~PlayerController();
	void pre_step(GameState* gs);

	void register_player(obj_id player);
	void remove_player(obj_id player);
	bool seen_by_player(GameState* gs, int pindex, GameInst* obj);

	const std::vector<obj_id>& player_ids() { return pids; }
	const std::vector<fov*>& player_fovs() { return fovs; }

private:
	std::vector<fov*> fovs;
	std::vector<obj_id> pids;
};

#endif /* PLAYERCONTROLLER_H_ */
