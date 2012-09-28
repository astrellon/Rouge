#include "test_lua_stats.h"

#include <tests/asserts.h>

#include <base/handle.h>
using namespace am::base;

#include <lua/lua_state.h>
using namespace am::lua;

#include <game/stats.h>
using namespace am::game;

#include <lua/wrappers/lua_stats.h>
using namespace am::lua::game;

extern "C" 
{ 
#	include <lua/src/lua.h>
#	include <lua/src/lauxlib.h>
#	include <lua/src/lualib.h>
} 

namespace am {
namespace tests {

	bool TestLuaStats::testSimple() {
		LuaState lua;
		
		

		return true;
	}

}
}