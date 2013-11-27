#include "test_lua_sound.h"

#include <tests/asserts.h>

#include <base/handle.h>

#include <lua/lua_state.h>
using namespace am::lua;

#include <sfx/sfx_isound.h>
#include <sfx/sfx_engine.h>
using namespace am::sfx;

#include <lua/wrappers/sfx/lua_sound.h>
using namespace am::lua::sfx;

extern "C" 
{ 
#	include <lua/src/lua.h>
#	include <lua/src/lauxlib.h>
#	include <lua/src/lualib.h>
} 

namespace am {
namespace tests {

	bool TestLuaSound::testSimple() {
		LuaState lua;
		
		assert(lua.loadString("Sound = import(\"Sound\")\n"
			//"testSound = Sound.new(\"data/
			));

		assert(lua.hasGlobalFunction("getName"));
		lua_acall(lua, 0, 1);
		am_equalsStr("testType", lua_tostring(lua, -1));
		lua.pop(1);

		assert(lua.hasGlobalFunction("getFullName"));
		lua_acall(lua, 0, 1);
		am_equalsStr("Test Type", lua_tostring(lua, -1));
		lua.pop(1);

		assert(lua.hasGlobalFunction("setFullName"));
		lua.push("New Test Type");
		lua_acall(lua, 1, 0);

		assert(lua.hasGlobalFunction("getFullName"));
		lua_acall(lua, 0, 1);
		am_equalsStr("New Test Type", lua_tostring(lua, -1));
		lua.pop(1);

		return true;
	}

}
}