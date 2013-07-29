#include "lua_return_codes.h"

extern "C" 
{ 
#	include <lua/src/lua.h>
#	include <lua/src/lauxlib.h>
#	include <lua/src/lualib.h>
}

#include <lua/lua_state.h>
using namespace am::lua;

#include <base/return_codes.h>
using namespace am::base;

namespace am {
namespace lua {
	/**
	 * @class
	 * A helper class for debugging Lua code.
	 * This class provides the follow return code constants.
	 * <ul>
	 * <li>success</li>
	 * <li>nil_parameter</li>
	 * <li>internal_error</li>
	 * <li>able_to_equip</li>
	 * <li>can_equip</li>
	 * <li>body_part_type_mismatch</li>
	 * <li>not_enough_body_parts</li>
	 * <li>out_of_range</li>
	 * <li>no_weapon_part</li>
	 * <li>body_part_found</li>
	 * <li>body_part_not_found</li>
	 * </ul>
	 */
	int ReturnCodes_register(lua_State *lua)
	{
		luaL_Reg regs[] = 
		{
			{ "message", ReturnCodes_message },
			{ NULL, NULL }
		};

		lua_pushstring(lua, "codes");
		lua_newtable(lua);

		luaL_setfuncs(lua, regs, 0);

		LuaState L(lua);
		L.setTableValue("success", SUCCESS);
		L.setTableValue("nil_parameter", NULL_PARAMETER);
		L.setTableValue("internal_error", INTERNAL_ERROR);
		L.setTableValue("able_to_equip", ABLE_TO_EQUIP);
		L.setTableValue("can_equip", CAN_EQUIP);
		L.setTableValue("body_part_type_mismatch", BODY_PART_TYPE_MISMATCH);
		L.setTableValue("not_enough_body_parts", NOT_ENOUGH_BODY_PARTS);
		L.setTableValue("out_of_range", OUT_OF_RANGE);
		L.setTableValue("no_weapon_part", NO_WEAPON_PART);
		L.setTableValue("body_part_found", BODY_PART_FOUND);
		L.setTableValue("body_part_not_found", BODY_PART_NOT_FOUND);

		lua_settable(lua, -3);

		return 0;
	}

	/**
	 * @static
	 * Takes a variable number of arguments and prints them to the logger.
	 * 
	 * @param ... args A variable number of Lua objects to log.
	 */
	inline int ReturnCodes_message(lua_State *lua)
	{
		if (lua_isnum(lua, 1))
		{
			lua_pushstring(lua, getErrorMessage(static_cast<ReturnCode>(lua_tointeger(lua, 1))));
			return 1;
		}
		return LuaState::expectedArgs(lua, "message", "am.code code_value");
	}

}
}
