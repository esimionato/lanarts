#ifndef SPRITEENTRY_H_
#define SPRITEENTRY_H_

#include <vector>
#include <string>

#include <lcommon/geometry.h>
#include <ldraw/Colour.h>
#include <ldraw/Drawable.h>

#include "data/FilenameList.h"//For FilenameList
#include "lanarts_defines.h"

struct SpriteEntry {
	enum sprite_type {
		ANIMATED, DIRECTIONAL
	};
	sprite_type type;
	Colour drawcolour;
	std::string name;
	ldraw::Drawable sprite;

	int width() const {
		return size().w;
	}

	int height() const {
		return size().h;
	}

	Size size() const {
		SizeF sz = sprite.size();
		return Size(sz.w, sz.h);
	}

	SpriteEntry(const std::string& name, const ldraw::Drawable& sprite,
			sprite_type type = ANIMATED, Colour drawcolour = Colour()) :
			type(type), name(name), sprite(sprite), drawcolour(drawcolour) {
	}
};

extern std::vector<SpriteEntry> game_sprite_data;

namespace res {
	ldraw::Drawable& sprite(sprite_id idx);

	ldraw::Drawable& sprite(const char* name);
	ldraw::Drawable& sprite(const std::string& name);

	sprite_id spriteid(const char* name);
	sprite_id spriteid(const std::string& name);
}

#endif /* SPRITEENTRY_H_ */
