#pragma once

#include <string>
#include <map>
using namespace std;

#include <base/handle.h>
#include <base/imanaged.h>
using namespace am::base;

#include <util/idefinition.h>
using namespace am::util;

#include <lua/lua_state.h>
using namespace am::lua;

#include "tile.h"

namespace am {
namespace game {

	class TileSet : public IManaged, public IDefinition {
	public:
		TileSet();
		TileSet(const char *name);
		~TileSet();

		void setName(const char *name);
		string getName() const;

		void setFullName(const char *fullName);
		string getFullName() const;

		void addTile(Tile *tile);
		bool addTileWithCheck(Tile *tile);
		void removeTile(Tile *tile);
		void removeTile(const char *tileName);
		bool hasTile(Tile *tile);
		bool hasTile(const char *tileName);
		Tile *getTile(const char *tileName);

		//void loadDef(JsonValue value);
		void loadDef(LuaState &lua);

		static const int LUA_ID;
		static const char *LUA_TABLENAME;

	protected:

		typedef map<string, Handle<Tile> > TileMap;
		TileMap mTiles;
		
		string mName;
		string mFullName;
	};

}
}
