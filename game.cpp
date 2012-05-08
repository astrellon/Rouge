#ifndef _ANDROID
#	include "game.h"
#endif

#include "map.h"
#include "json_value.h"

#include <stdio.h>

using namespace am::util;

namespace am {
namespace base {

Game::Game() :
	mMap(NULL),
	mMapFilename("")
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

	FILE *file = fopen(filename.c_str(), "r");
	if (file == NULL) {
		return -1;
	}

	fseek(file, 0, SEEK_END);
	long fileLen = ftell(file);
	rewind(file);

	char *buff = new char[fileLen + 1];
	fread(buff, 1, fileLen, file);
	buff[fileLen] = '\0';

	JsonValue loaded = JsonValue::import(buff);

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
