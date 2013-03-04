#pragma once

extern "C" 
{
#	include <lua/src/lstate.h>
}

namespace am {
namespace game {
	class TileSet;
	class TileSetType;
}

namespace lua {
namespace game {

	int TileSet_ctor(lua_State *lua);
	int TileSet_dtor(lua_State *lua);
	int TileSet_eq(lua_State *lua);
	int TileSet_register(lua_State *lua);

	int TileSet_name(lua_State *lua);
	int TileSet_full_name(lua_State *lua);

	int TileSet_add_tile(lua_State *lua);
	int TileSet_remove_tile(lua_State *lua);
	int TileSet_has_tile(lua_State *lua);
	int TileSet_tile(lua_State *lua);

	int TileSet_load_def(lua_State *lua);

}
}
}
