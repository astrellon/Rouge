#pragma once

#include <map>
#include <string>
#include <vector>
using namespace std;

#include "handle.h"
using namespace am::util;

namespace am {

namespace base {

	class Game;
	class Tile;

	class Engine {
	public:
		Engine();
		~Engine();

		void init();
		void deinit();
		void update(float dt);
		void mouseFunc(int mouseButton, int x, int y);
		void keyboardFunc(const bool *keys, int key);

		void setCurrentGame(Game *game);
		Game *getCurrentGame();

	protected:

		Handle<Game> mCurrentGame;

		//void registerTile(Tile *t);
		//void deregisterTiles();

		//typedef map<string, Tile *> TileMap; 

		//TileMap mRegTiles;
	};

}
}
