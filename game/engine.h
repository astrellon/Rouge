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

#include <math/vector.h>
#include <math/math.h>
using namespace am::math;

#include <util/idefinition_manager.h>
using namespace am::util;

#include "tile_set.h"
#include "race.h"
#include "tile_type.h"

namespace am {
namespace sys {
	class ISystem;
}
using namespace am::sys;

namespace game {

	class Game;
	class GameObject;
	class Tile;

	typedef map<string, Handle<TileSet> > TileSetMap;
	typedef map<string, Handle<Race> > RaceMap;
	typedef map<string, Handle<TileType> > TileTypeMap;

	class Engine : public EventInterface, public IDefinitionManager {
	public:
		Engine();
		~Engine();

		void init();
		void deinit();
		void update(float dt);

		void setCurrentGame(Game *game);
		Game *getCurrentGame();

		void setGridSize(float size);
		float getGridSize() const;
		float getGridSizeResp() const;

		void usingTileSet(const char *tileSetName);
		void clearUsingTileSet();

		Tile *getTile(const char *name);

		TileSet *getTileSet(const char *tileSetName);
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
		bool loadGame(const char *saveName);

		Vector2f gridToWorld(const Vector2i &grid) const;
		Vector2i worldToGrid(const Vector2f &world) const;

		static Engine *getEngine();
		static void setEngine(Engine *engine);

		static float gridSize();
		static float gridSizeResp();

		static Game *getGame();

		static const int LUA_ID;
		static const char *LUA_TABLENAME;

	protected:

		Handle<Game> mCurrentGame;

		float mGridSize;

		float mGridSizeResp;

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
		virtual const char *getBaseDefinitionPath(int id) const;

		static Engine *sMainEngine;
	};

}
}
