#ifndef _ANDROID
#	include "engine.h"
#endif

#include "game.h"
#include "tile.h"
#include "json_value.h"

using namespace am::util;

namespace am {
namespace base {

Engine::Engine() {
}
Engine::~Engine() {
	deregisterTiles();
}

Game *Engine::createGame() {
	return new Game(this);
}

Tile *Engine::getRegTile(const char *name) {
	return getRegTile(string(name));
}
Tile *Engine::getRegTile(const string &tile) {
	TileMap::iterator iter = mRegTiles.find(tile);
	if (iter == mRegTiles.end()) {
		return NULL;
	}
	return iter->second;
}

int Engine::registerTiles(const char *filename) {

	JsonValue loaded = JsonValue::import_from_file(filename);
	if (loaded.getType() == JV_INT) {
		int error = loaded.getInt();
		if (error == -1) {
			printf("Unable to load tile definitions '%s'\n", filename);
			return -1;
		}
		if (error == -2) {
			printf("Unable to allocate space for tile definitions '%s'\n", filename);
			return -2;
		}
		printf("Unknown error for tile definitions '%s': %d\n", filename, error);
		return error;
	}
	if (loaded.getType() != JV_OBJ) {
		printf("Loaded tile definition data (%s) not an Object!\n", loaded.getTypeName());
		return -3;
	}

	JsonObject *obj = loaded.getObj();

	JsonObject::iterator iter;
	for (iter = obj->begin(); iter != obj->end(); ++iter) {
		JsonValue tileDef = iter->second;
		if (tileDef.getType() != JV_OBJ) {
			// If it's just a comment then we just need to skip;
			if (iter->first.compare("__comment") != 0) {
				printf("Tile def '%s', not an Object (%s)\n", 
					iter->first.c_str(), tileDef.getTypeName());
			}
			continue;
		}

		Tile *t = new Tile(iter->first.c_str());
		t->loadDef(*tileDef.getObj());
		registerTile(t);
	}

	return 0;
}
void Engine::registerTile(Tile *t) {
	mRegTiles[t->getName()] = t;
}
void Engine::deregisterTiles() {
	TileMap::iterator iter;
	for (iter = mRegTiles.begin(); iter != mRegTiles.end(); ++iter) {
		if (iter->second) {
			delete iter->second;
		}
	}
	mRegTiles.clear();
}

}
}
