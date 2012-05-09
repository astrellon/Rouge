#ifndef _ANDROID
#	include "game.h"
#endif

#include "map.h"
#include "json_value.h"
#include "tile_instance.h"

#include <stdio.h>

using namespace am::util;

namespace am {
namespace base {

Game::Game(Engine *engine) :
	mMap(NULL),
	mMapFilename(""),
	mEngine(engine)
{

}
Game::~Game() {

}

void Game::setMap(Map *map) {
	mMap = map;
}
Map *Game::getMap() {
	return mMap;
}

int Game::loadMap(const string &filename) {
	mMapFilename = filename;

	JsonValue loaded = JsonValue::import_from_file(filename.c_str());
	if (loaded.getType() == JV_INT) {
		int error = loaded.getInt();
		if (error == -1) {
			printf("Unable to load map '%s'\n", filename.c_str());
			return -1;
		}
		if (error == -2) {
			printf("Unable to allocate space for input map '%s'\n", filename.c_str());
			return -2;
		}
		printf("Unknown error for input map '%s': %d\n", filename.c_str(), error);
		return error;
	}

	int mapWidth = -1;
	int mapHeight = -1;

	loaded.display();

	if (loaded.has("map", JV_OBJ)) {
		JsonValue mapData = loaded["map"];
		
		if (mapData.has("data", JV_ARR)) {
			JsonArray *mapRows = mapData["data"].getArr();
			mapHeight = static_cast<int>(mapRows->size());

			int y = -1;

			JsonArray::iterator row;
			for (row = mapRows->begin(); row != mapRows->end(); ++row) {
				y++;

				if (row->getType() != JV_ARR) {
					printf("Error in map data!\n");
					break;
				}
				JsonArray *mapCols = row->getArr();
				JsonArray::iterator col;

				int x = -1;
				for (col = mapCols->begin(); col != mapCols->end(); ++col) {
					x++;
					if (col->getType() != JV_OBJ) {
						printf("Map tile instance not an object: '%s'\n", col->getTypeName());
						break;
					}
					if (!col->has("tile", JV_STR)) {
						printf("Map tile instance does not have a tile property!\n");
						break;
					}
					
					JsonObject *tileInstObj = col->getObj();
					printf("Tile instance: '%s' at %d, %d\n", col->at("tile").getCStr(), x, y);
					//TileInstance *tileInst = new TileInstance(
				}
			}
		}
	}

	printf("Loaded map data: %d, %d\n", mapWidth, mapHeight);

	return 0;
}
string Game::getMapFilename() const {
	return mMapFilename;
}

}
}
