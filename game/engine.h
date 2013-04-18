#pragma once

#include <map>
#include <string>
#include <vector>
using namespace std;

#include <base/handle.h>
using namespace am::base;

#include <ui/ui_game_hud.h>
#include <ui/event_interface.h>
using namespace am::ui;

#include <lua/lua_state.h>
using namespace am::lua;

#include "tile_set.h"

namespace am {
namespace sys {
	class ISystem;
}
using namespace am::sys;

namespace game {

	class Game;
	class GameObject;
	class Tile;
	class TileType;
	class Race;

	typedef map<string, Handle<TileSet> > TileSetMap;
	typedef map<string, Race *> RaceMap;
	typedef map<string, TileType *> TileTypeMap;

	class Engine : public EventInterface {
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

		TileSet *getTileSetLua(const char *tileSetName);
		void addTileSet(TileSet *tileSet);
		TileSetMap &getTileSets();
		TileSet *getTopLevelTileSet();

		void setGameHud(GameHud *hud);
		GameHud *getGameHud();

		// GameObjects shortcuts.
		GameObject *getGameObject(const char *id) const;
		bool registerGameObject(GameObject *obj);
		void deregisterGameObject(const char *id);
		void deregisterGameObject(GameObject *obj);

		// Race
		bool addRace(Race *race);
		bool removeRace(Race *race);
		Race *getRace(const char *raceName);
		Race *getUnknownRace();

		// TileType
		void addTileType(TileType *type);
		TileType *getTileType(const char *name);
		TileType *getTileType(const string &name);

		LuaState &getLua();
		bool loadLuaEngine(const char *scriptName);

		bool newGame(const char *scenarioName);
		bool loadGame(const char *scenarioName, const char *saveName);

		static Engine *getEngine();
		static void setEngine(Engine *engine);

		static Game *getGame();

		static const int LUA_ID;
		static const char *LUA_TABLENAME;

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
		TileTypeMap mTileTypes;

		Handle<GameHud> mGameHud;

		RaceMap mRaces;
		Race *mUnknownRace;

		LuaState mLua;

		void checkUsingTileSet();

		static Engine *sMainEngine;
	};

}
}
