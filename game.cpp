#ifndef _ANDROID
#	include "game.h"
#endif

#include "map.h"
#include "json_value.h"

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

	if (loaded.has("map", JV_OBJ)) {
		JsonValue mapData = loaded["map"];
		if (mapData.has("width", JV_INT)) {
			mapWidth = mapData["width"].getInt();
		}
		if (mapData.has("height", JV_INT)) {
			mapHeight = mapData["height"].getInt();
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
