#pragma once

#include <map>
#include <string>
#include <vector>
using namespace std;

#include "handle.h"
using namespace am::util;

#include "screen.h"

namespace am {

namespace base {

	class Game;
	class Tile;
	class Screen;

	class Engine {
	public:
		Engine();
		~Engine();

		Screen *getScreen(const char *screenName);
		Screen *getScreen(const string &screenName);

		void init();
		void deinit();
		void update(float dt);
		void mouseFunc(int mouseButton, int x, int y);
		void keyboardFunc(const bool *keys, int key);

		void setCurrentGame(Game *game);
		Game *getCurrentGame();

	protected:

		Handle<Game> mCurrentGame;

		typedef map<string, Handle<Screen> > ScreenMap;
		ScreenMap mScreens;

		//void registerTile(Tile *t);
		//void deregisterTiles();

		//typedef map<string, Tile *> TileMap; 

		//TileMap mRegTiles;
	};

}
}
