#pragma once

#include <map>
#include <string>
#include <vector>
using namespace std;

#include <base/handle.h>
using namespace am::base;

#include <ui/ui_game_hud.h>
using namespace am::ui;

#include "tile_set.h"

namespace am {
namespace game {

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
		float getGridXSizeResp() const;

		void setGridYSize(float size);
		float getGridYSize() const;
		float getGridYSizeResp() const;

		void usingTileSet(const char *tileSetName);
		void clearUsingTileSet();

		Tile *getTile(const char *name);

		TileSet *getTileSet(const char *tileSetName);
		TileSet *getTileSetLua(const char *tileSetName);
		void addTileSet(TileSet *tileSet);
		TileSetMap &getTileSets();
		TileSet *getTopLevelTileSet();

		void setGameHud(GameHud *hud);
		GameHud *getGameHud();

		static Engine *getEngine();
		static void setEngine(Engine *engine);

	protected:

		Handle<Game> mCurrentGame;

		float mGridXSize;
		float mGridYSize;

		float mGridXSizeResp;
		float mGridYSizeResp;

		typedef map<string, int> UsingTileSet;
		UsingTileSet mUsingTileSetNames;
		bool mUsingTileSetDirty;
		TileSetMap mUsingTileSet;

		TileSetMap mTileSets;
		Handle<TileSet> mTopLevelTileSet;

		Handle<GameHud> mGameHud;

		void checkUsingTileSet();

		static Engine *sMainEngine;
	};

}
}
