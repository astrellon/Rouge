#pragma once

#include <map>
#include <string>
#include <vector>
using namespace std;

#include "handle.h"
using namespace am::util;

#include "tile_set.h"

namespace am {
namespace base {

	class Game;
	class Tile;

	typedef map<string, Handle<TileSet> > TileSetMap;

	class Engine {
	public:
		Engine();
		~Engine();

		void init();
		void deinit();
		void update(float dt);

		void setCurrentGame(Game *game);
		Game *getCurrentGame();

		void setGridXSize(float size);
		float getGridXSize() const;

		void setGridYSize(float size);
		float getGridYSize() const;

		void usingTileSet(const char *tileSetName);
		void clearUsingTileSet();

		Tile *getTile(const char *name);

		TileSet *getTileSet(const char *tileSetName);
		void addTileSet(TileSet *tileSet);
		TileSetMap &getTileSets();
		TileSet *getTopLevelTileSet();

		static Engine *getMainEngine();
		static void setMainEngine(Engine *engine);

	protected:

		Handle<Game> mCurrentGame;

		float mGridXSize;
		float mGridYSize;

		typedef map<string, int> UsingTileSet;
		UsingTileSet mUsingTileSetNames;
		bool mUsingTileSetDirty;
		TileSetMap mUsingTileSet;

		TileSetMap mTileSets;
		Handle<TileSet> mTopLevelTileSet;

		void checkUsingTileSet();

		static Engine *sMainEngine;
	};

}
}
