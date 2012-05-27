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

	void init();
	void deinit();
	void update(float dt);
	void mouseFunc(int mouseButton, int x, int y);
	void keyboardFunc(const bool *keys, int key);

protected:

	void registerTile(Tile *t);
	void deregisterTiles();

	typedef map<string, Tile *> TileMap; 

	TileMap mRegTiles;
};

}
}
