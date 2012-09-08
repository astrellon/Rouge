#pragma once

extern "C" 
{ 
#	include <lua/src/lua.h>
#	include <lua/src/lauxlib.h>
#	include <lua/src/lualib.h>
} 

#include <string>
using namespace std;

#include <base/imanaged.h>
using namespace am::base;

namespace am {
namespace lua {

	class LuaState : public IManaged {
	public:
		LuaState();
		~LuaState();

		static int onError(lua_State *mLua);

	protected:
		lua_State *mLua;

	};

}
}
