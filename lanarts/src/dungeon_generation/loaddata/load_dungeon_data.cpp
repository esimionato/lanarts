/*
 * load_dungeon_data.cpp:
 *  Load dungeon area & level data
 */

#include <fstream>

#include <yaml-cpp/yaml.h>

#include "lua_api/lua_yaml.h"
#include "data/game_data.h"
#include "data/parse_with_defaults.h"
#include "data/yaml_util.h"

using namespace std;

itemgenlist_id parse_itemgenlist_id(const YAML::Node& n) {
	std::string name = parse_str(n);
	return get_itemgenlist_by_name(name.c_str());
}

ItemGenSettings parse_item_gen(const YAML::Node& n) {
	ItemGenSettings igs;
	igs.num_items = parse_range(n["amount"]);

	if (yaml_has_node(n, "itemgenlist")) {
		igs.itemgenlist = parse_itemgenlist_id(n["itemgenlist"]);
	}
	return igs;
}
RegionGenSettings parse_room_gen(const YAML::Node& n) {
	RegionGenSettings rgs;
	rgs.amount_of_regions = parse_range(n["amount"]);
	rgs.region_padding = parse_defaulted(n, "padding", 1);
	rgs.size = parse_range(n["size"]);
	return rgs;
}
TunnelGenSettings parse_tunnel_gen(const YAML::Node& n) {
	TunnelGenSettings tgs;
	tgs.size = parse_range(n["width"]);
	tgs.num_tunnels = parse_range(n["per_room"]);
	tgs.padding = parse_defaulted(n, "padding", 1);

	return tgs;
}

static void push_tileset_node(std::vector<tileset_id>& tilesets,
		const YAML::Node& n) {
	std::string tilesetname = parse_str(n);
	tilesets.push_back(get_tileset_by_name(tilesetname.c_str()));
}

FeatureGenSettings parse_feature_gen(const YAML::Node& n) {
	FeatureGenSettings fgs;
	fgs.nstairs_up = parse_defaulted(n, "stairs_up", 3);
	fgs.nstairs_down = parse_defaulted(n, "stairs_down", 3);
	if (yaml_has_node(n, "tileset")) {
		const YAML::Node& tilenode = n["tileset"];
		if (tilenode.Type() == YAML::NodeType::Scalar) {
			push_tileset_node(fgs.tilesets, tilenode);
		} else {
			for (int i = 0; i < tilenode.size(); i++) {
				push_tileset_node(fgs.tilesets, tilenode[i]);
			}
		}
	}
	return fgs;
}

EnemyGenChance parse_enemy_chance(const YAML::Node& n) {
	EnemyGenChance egc;
	egc.enemytype = parse_enemy_number(n, "enemy");
	egc.generate_chance = parse_defaulted(n, "chance", 0);
	egc.guaranteed_spawns = parse_defaulted(n, "guaranteed_spawns", Range());
	egc.generate_group_chance = parse_defaulted(n, "group_chance", 0);
	egc.groupsize = parse_defaulted(n, "group_size", Range(2, 2));
	return egc;
}
EnemyGenSettings parse_enemy_gen(const YAML::Node& node, const char* key) {
	EnemyGenSettings egs;
	if (yaml_has_node(node, key)) {
		const YAML::Node& n = node[key];
		egs.wandering = parse_defaulted(n, "wandering", true);

		egs.num_monsters = parse_range(n["amount"]);
		if (yaml_has_node(n, "generated")) {
			egs.enemy_chances = parse_named_with_defaults(n["generated"],
					"enemy", &parse_enemy_chance);
		}
	}
	return egs;
}

ContentGenSettings parse_content_gen(const YAML::Node& n) {
	ContentGenSettings cgs;
	cgs.items = parse_item_gen(n["items"]);
	cgs.features = parse_feature_gen(n["features"]);
	cgs.enemies = parse_enemy_gen(n, "enemies");
	return cgs;
}

LayoutGenSettings parse_layout_gen(const YAML::Node& n) {
	LayoutGenSettings lgs;

	lgs.solid_fill = parse_defaulted(n, "solid_fill", true);
	lgs.tunnels = parse_tunnel_gen(n["tunnels"]);
	const YAML::Node& size = n["size"];
	lgs.width = parse_range(size[0]);
	lgs.height = parse_range(size[1]);

	const YAML::Node& rooms = n["rooms"];
	if (rooms.Type() == YAML::NodeType::Sequence) {
		for (int i = 0; i < rooms.size(); i++) {
			lgs.rooms.push_back(parse_room_gen(rooms[i]));
		}
	} else {
		lgs.rooms.push_back(parse_room_gen(rooms));
	}

	return lgs;
}

RoomGenSettings parse_level_gen(lua_State* L, const YAML::Node& n) {
	RoomGenSettings level;
	level.content = parse_content_gen(n["content"]);

	const YAML::Node& layouts = n["layout"];
	if (layouts.Type() == YAML::NodeType::Sequence) {
		for (int i = 0; i < layouts.size(); i++) {
			level.layouts.push_back(parse_layout_gen(layouts[i]));
		}
	} else {
		level.layouts.push_back(parse_layout_gen(layouts));
	}
	if (yaml_has_node(n, "templates")) {
		const YAML::Node& templates = n["templates"];
		for (int i = 0; i < templates.size(); i++) {
			std::string template_name = parse_str(templates[i]);
			level.area_templates.push_back(
					get_area_template_by_name(template_name.c_str()));
		}
	}
	level.gen_level_func = parse_luaexpr(L, n, "gen_level_func");
	return level;

}
void parse_dungeon_branch(lua_State* L, const YAML::Node& n,
		std::vector<RoomGenSettings>& rooms) {
	const YAML::Node& lnodes = n["rooms"];
	for (int i = 0; i < lnodes.size(); i++) {
		//printf("Loading level\n");
		rooms.push_back(parse_level_gen(L, lnodes[i]));
	}
}

void load_dungeon_callbackf(const YAML::Node& node, lua_State* L,
		LuaValue* value) {
//	game_class_data.push_back( parse_class(node) );
}

LuaValue load_dungeon_data(lua_State* L, const FilenameList& filenames) {
	LuaValue ret;
	try {

		std::string fname = "res/" + filenames.at(0);
		fstream file(fname.c_str(), fstream::in | fstream::binary);

		YAML::Parser parser(file);
		YAML::Node root;

		parser.GetNextDocument(root);

		const YAML::Node& node = root["areas"];
		//First branch should be main branch, using node[0]:
		parse_dungeon_branch(L, node[0], game_dungeon_yaml);
		//TODO: remove this hack when there are proper subbranches
		game_dungeon_yaml[0].content.features.nstairs_up = 0;

		game_dungeon_data[0] = DungeonBranch(&game_dungeon_yaml[0],
				game_dungeon_yaml.size());
		file.close();
	} catch (const YAML::Exception& parse) {
		printf("Dungeon Parsed Incorrectly: \n");
		printf("%s\n", parse.what());
		fflush(stdout);
	}
	return ret;
}
