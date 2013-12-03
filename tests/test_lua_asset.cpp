#include "test_lua_asset.h"

#include <tests/asserts.h>

#include <base/handle.h>

#include <lua/lua_state.h>

extern "C" 
{ 
#	include <lua/src/lua.h>
#	include <lua/src/lauxlib.h>
#	include <lua/src/lualib.h>
} 

namespace am {
namespace tests {

	bool TestLuaAsset::testSimple()
	{
		lua::LuaState lua;
		
		bool loadResult = lua.loadString(
			"asset = am.asset.new(\"cursor\")\n"
			"am.debug.equals(\"cursor\", asset:name())\n"
			"asset, result = asset:texture(\"data/textures/cursor.png\")\n"
			"am.debug.equals(am.code.success, result)\n"
			"am.debug.equals(true, asset:has_texture(\"data/textures/cursor.png\"))\n"
			"am.debug.equals(1, asset:total_textures())\n"
			"asset:num_frames(4, 2)\n"
			"am.debug.equals(8, asset:total_texture_windows())\n"
			"asset:total_texture_windows(7)\n"
			"am.debug.equals(7, asset:total_texture_windows())\n"
			
			"am.debug.equals(\"none\", asset:scale_nine_state())\n"
			"scale_nine = asset:scale_nine()\n"
			"am.debug.equals(0, scale_nine.left_x)\n"
			"am.debug.equals(0, scale_nine.top_y)\n"
			"am.debug.equals(0, scale_nine.right_x)\n"
			"am.debug.equals(0, scale_nine.bottom_y)\n"

			"scale_nine_set = { left_x = 3, right_x = 7, top_y = 2, bottom_y = 5 }\n"
			"asset:scale_nine(scale_nine_set, true)\n"
			"am.debug.equals(\"both\", asset:scale_nine_state())\n"
			"scale_nine = asset:scale_nine()\n"
			"am.debug.equals(3, scale_nine.left_x)\n"
			"am.debug.equals(2, scale_nine.top_y)\n"
			"am.debug.equals(7, scale_nine.right_x)\n"
			"am.debug.equals(5, scale_nine.bottom_y)\n"

			"scale_nine_set = { left_x = 1, right_x = 8}\n"
			"asset:scale_nine(scale_nine_set, true)\n"
			"am.debug.equals(\"horizontal\", asset:scale_nine_state())\n"
			"scale_nine = asset:scale_nine()\n"
			"am.debug.equals(1, scale_nine.left_x)\n"
			"am.debug.equals(8, scale_nine.right_x)\n"

			"scale_nine_set = { top_y = 4, bottom_y = 9}\n"
			"asset:scale_nine(scale_nine_set, true)\n"
			"am.debug.equals(\"vertical\", asset:scale_nine_state())\n"
			"scale_nine = asset:scale_nine()\n"
			"am.debug.equals(4, scale_nine.top_y)\n"
			"am.debug.equals(9, scale_nine.bottom_y)\n"

			"scale_nine_set = { left_x = 2, top_y = 4, bottom_y = 9}\n"
			"asset:scale_nine(scale_nine_set, true)\n"
			"am.debug.equals(\"vertical\", asset:scale_nine_state())\n"
			"scale_nine = asset:scale_nine()\n"
			"am.debug.equals(4, scale_nine.top_y)\n"
			"am.debug.equals(9, scale_nine.bottom_y)\n"
			"am.debug.equals(2, scale_nine.left_x)\n"
			);
		if (!loadResult)
		{
			lua.logStack("TESTERR");
			assert(loadResult);
		}

		return true;
	}

}
}

