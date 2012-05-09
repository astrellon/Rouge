#pragma once

#include <map>
#include <string>
#include <vector>

using namespace std;

namespace am {
namespace base {

class Game;
class Tile;

class Engine {
public:
	Engine();
	~Engine();

	Game *createGame();

	int registerTiles(const char *filename);

	Tile *getRegTile(const char *name);
	Tile *getRegTile(const string &name);

protected:

	void registerTile(Tile *t);
	void deregisterTiles();

	typedef map<string, Tile *> TileMap; 

	TileMap mRegTiles;
};

}
}
