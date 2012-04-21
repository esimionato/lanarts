/*
 * load_sprite_data.cpp
 *
 *  Created on: Mar 25, 2012
 *      Author: 100397561
 */
#include <fstream>

#include "../game_data.h"
#include <yaml-cpp/yaml.h>
#include "yaml_util.h"

using namespace std;

static GenRange parse_tile_range(const YAML::Node& n){
	GenRange gr;
	if (n.Type() == YAML::NodeType::Sequence){
		std::vector<std::string> components;
		n >> components;
		gr.min = get_tile_by_name(components[0].c_str());
		gr.max = get_tile_by_name(components[1].c_str());
	} else {
		std::string s;
		n >> s;
		gr.min = get_tile_by_name(s.c_str());
		gr.max = gr.min;
	}
	return gr;
}

static TilesetEntry parse_tilesetentry(const YAML::Node& node){
	std::string name;
	node["tileset"] >> name;
	GenRange floor = parse_tile_range(node["floor_tile"]);
	GenRange wall = parse_tile_range(node["wall_tile"]);
	GenRange corridor = parse_tile_range(node["corridor_tile"]);
	GenRange altfloor = parse_tile_range(node["alt_floor_tile"]);
	GenRange altwall = parse_tile_range(node["alt_wall_tile"]);
	GenRange altcorridor = parse_tile_range(node["alt_corridor_tile"]);
	return TilesetEntry(
			name,
			floor.min,floor.max,
			wall.min,wall.max,
			corridor.min,corridor.max,
			altfloor.min,altfloor.max,
			altwall.min,altwall.max,
			altcorridor.min,altcorridor.max);
}

void load_tileset_data(const char* filename){

    fstream file(filename, fstream::in | fstream::binary);
	game_tileset_data.clear();

	try{
		YAML::Parser parser(file);
		YAML::Node root;

		parser.GetNextDocument(root);

		const YAML::Node& node = root["tilesets"];
	
		for(int i = 0; i < node.size(); i++){
			game_tileset_data.push_back(parse_tilesetentry(node[i]));
		}
	} catch (const YAML::Exception& parse){
		printf("Enemies Parsed Incorrectly: \n");
		printf("%s\n", parse.what());
	}
	file.close();

}
