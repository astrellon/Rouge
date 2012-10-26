#pragma once

#include <map>
#include <string>
using namespace std;

#include <util/json_value.h>
using namespace am::util;

#include <lua/lua_state.h>
using namespace am::lua;

namespace am {
namespace game {

	class TileType {
	public:
		TileType(const char *name, const char *fullName = NULL);
		~TileType();

		const char *getName() const;

		void setFullName(const char *name);
		const char *getFullName() const;

		bool loadFromDef(JsonValue value);
		bool loadFromDef(LuaState &lua);
		
		static void addTileType(TileType *type);
		static TileType *getTileType(const char *name);
		static TileType *getTileType(const string &name);

		static bool loadStandardTileTypes(const char *filename);
		static bool loadStandardTileTypesLua(const char *filename);
		
	protected:

		string mName;
		string mFullName;
	
		typedef map<string, TileType *> TileTypeMap;
		static TileTypeMap sTileTypes;
		
	};

}
}
