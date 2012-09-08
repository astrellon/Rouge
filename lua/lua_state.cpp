#include "lua_state.h"

namespace am {
namespace lua {

	LuaState::LuaState()
	{
		mLua = luaL_newstate(); 
		luaL_openlibs(mLua);
		lua_pushcfunction(mLua, onError);
	}
	LuaState::~LuaState()
	{
		lua_close(mLua);
	}

	int LuaState::onError(lua_State *lua)
	{
		return 0;
	}

}
}
