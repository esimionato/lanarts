/*
 * game_data.h
 *
 *  Created on: 2011-10-27
 *      Author: 100397561
 */

#ifndef GAME_DATA_H_
#define GAME_DATA_H_

#include "class_data.h"
#include "item_data.h"
#include "tile_data.h"
#include "tileset_data.h"
#include "spell_data.h"
#include "sprite_data.h"
#include "dungeon_data.h"
#include "enemy_data.h"
#include "weapon_data.h"
#include "../world/GameSettings.h"
#include "../util/LuaValue.h"

struct lua_State;

void load_tile_data(const char* filename);
void load_tileset_data(const char* filename);
LuaValue load_sprite_data(lua_State* L, const char* filename);
void load_weapon_data(lua_State* L, const char* filename, LuaValue* itemstable = NULL);
void load_weapon_item_entries(lua_State* L);

LuaValue load_item_data(lua_State* L, const char* filename);
LuaValue load_enemy_data(lua_State* L, const char* filename);
LuaValue load_dungeon_data(lua_State* L, const char* filename);
LuaValue load_class_data(lua_State* L, const char* filename);

GameSettings load_settings_data(const char* filename);

void init_game_data(lua_State* L);
void init_lua_data(GameState* gs, lua_State* L);

#endif /* GAME_DATA_H_ */
