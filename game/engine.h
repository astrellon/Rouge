#pragma once

#include <map>
#include <string>
#include <vector>

#include <base/handle.h>

#include <ui/ui_game_hud.h>
#include <ui/ui_editor_hud.h>
#include <ui/event_interface.h>

#include <lua/lua_state.h>

#include <math/vector.h>
#include <math/math.h>

#include <util/idefinition_manager.h>

#include "tile_set.h"
#include "race.h"
#include "tile_type.h"

namespace am {
namespace sys {
	class ISystem;
}

namespace game {

	class Game;
	class GameObject;
	class Tile;

	using base::Handle;
	using math::Vector2f;
	using math::Vector2i;

	typedef std::map<std::string, Handle<TileSet> > TileSetMap;
	typedef std::map<std::string, Handle<Race> > RaceMap;
	typedef std::map<std::string, Handle<TileType> > TileTypeMap;

	class Engine : public ui::EventInterface, public am::util::IDefinitionManager
	{
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

		TileSet *getTileSet(const char *tileSetName, bool reload=false);
		void addTileSet(TileSet *tileSet);
		TileSetMap &getTileSets();
		TileSet *getTopLevelTileSet();

		void setGameHud(ui::GameHud *hud);
		ui::GameHud *getGameHud() const;

		void setEditorHud(ui::EditorHud *hud);
		ui::EditorHud *getEditorHud() const;

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
		TileType *getTileType(const std::string &name);

		am::lua::LuaState &getLua();
		bool loadLuaEngine(const char *scriptName);

		bool newGame(const char *scenarioName);
		bool loadGame(const char *saveName);

		Vector2f gridToWorld(const Vector2i &grid) const;
		Vector2f gridToWorld(int x, int y) const;
		Vector2i worldToGrid(const Vector2f &world) const;
		Vector2i worldToGrid(float x, float y) const;

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

		typedef std::map<std::string, int> UsingTileSet;
		Engine::UsingTileSet mUsingTileSetNames;

		bool mUsingTileSetDirty;
		TileSetMap mUsingTileSet;

		TileSetMap mTileSets;
		Handle<TileSet> mTopLevelTileSet;
		TileTypeMap mTileTypes;

		Handle<ui::GameHud> mGameHud;
		Handle<ui::EditorHud> mEditorHud;

		RaceMap mRaces;
		Race *mUnknownRace;

		am::lua::LuaState mLua;

		void checkUsingTileSet();
		virtual const char *getBaseDefinitionPath(int id) const;

		static Engine *sMainEngine;
	};

}
}
