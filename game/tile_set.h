#pragma once

#include <string>
#include <map>

#include <base/handle.h>
#include <base/imanaged.h>

#include <util/idefinition.h>
using namespace am::util;

#include <lua/lua_state.h>
using namespace am::lua;

#include "tile.h"

namespace am {
namespace game {

	class TileSet : public am::base::IManaged, public IDefinition {
	public:

		typedef std::map<std::string, am::base::Handle<Tile> > TileMap;

		TileSet();
		TileSet(const char *name);
		~TileSet();

		void setName(const char *name);
		std::string getName() const;

		void setFullName(const char *fullName);
		std::string getFullName() const;

		void addTile(Tile *tile);
		bool addTileWithCheck(Tile *tile);
		void removeTile(Tile *tile);
		void removeTile(const char *tileName);
		bool hasTile(Tile *tile);
		bool hasTile(const char *tileName);
		Tile *getTile(const char *tileName);
		const TileMap &getTiles() const;

		static const int LUA_ID;
		static const char *LUA_TABLENAME;

	protected:

		TileMap mTiles;
		
		std::string mName;
		std::string mFullName;
	};

}
}
