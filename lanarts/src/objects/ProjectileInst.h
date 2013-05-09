/* ProjectileInst.h:
 *  Represents a projectile attack's trajectory
 */

#ifndef PROJECTILEINST_H_
#define PROJECTILEINST_H_

#include <cstdlib>

#include "stats/items/items.h"
#include "stats/stats.h"

#include "util/callback_util.h"
#include "lanarts_defines.h"

#include "GameInst.h"

class ProjectileInst: public GameInst {
	enum {
		DEPTH = 0
	};
public:
	ProjectileInst(const Item& projectile,
			const EffectiveAttackStats& atkstats, obj_id origin_id, const Pos& start,
			const Pos& target, float speed, int range, obj_id sole_target = 0, bool bounce = false, int hits = 1);
	~ProjectileInst();
	virtual void step(GameState* gs);
	virtual void draw(GameState* gs);
	virtual void deinit(GameState* gs);
	virtual void copy_to(GameInst* inst) const;
	virtual ProjectileInst* clone() const;

	virtual void serialize(GameState* gs, SerializeBuffer& serializer);
	virtual void deserialize(GameState* gs, SerializeBuffer& serializer);

	sprite_id sprite() const;
private:
	static bool bullet_target_hit2(GameInst* self, GameInst* other);

	float rx, ry, vx, vy, speed;
	/* Team alignment, to determine if friendly-firing */
	team_id team;
	/* Origin object, and optional exclusive target*/
	obj_id origin_id, sole_target;

	/* Projectile used */
	Item projectile;

	/* Stats at time of projectile creation */
	EffectiveAttackStats atkstats;

	/* Range left before projectile is destroyed */
	int range_left;

	/*TODO: move to lua*/
	bool bounce;
	int hits;
	float damage_mult;
};

#endif /* PROJECTILEINST_H_ */
