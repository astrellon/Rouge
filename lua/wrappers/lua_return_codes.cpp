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

namespace am {
namespace lua {
	/**
	 * @class
	 * A helper class for debugging Lua code.
	 * This class provides the follow return code constants.
	 * <h4>General</h4>
	 * <ul>
	 *  <li>success</li>
	 *  <li>nil_parameter</li>
	 *  <li>internal_error</li>
	 *  <li>file_not_found</li>
	 * </ul>
	 * <h4>Character</h4>
	 * <ul>
	 *  <li>able_to_equip</li>
	 *  <li>can_equip</li>
	 *  <li>body_part_type_mismatch</li>
	 *  <li>not_enough_body_parts</li>
	 *  <li>out_of_range</li>
	 *  <li>no_weapon_part</li>
	 *  <li>off_the_map</li>
	 *  <li>invalid_location</li>
	 * </ul>
	 * <h4>Body Parts</h4>
	 * <ul>
	 *  <li>body_part_found</li>
	 *  <li>body_part_not_found</li>
	 *  <li>body_part_type_unknown</li>
	 * </ul>
	 * <h4>Texture</h4>
	 * <ul>
	 *  <li>texture_load_failed</li>
	 * </ul>
	 * <h4>System</h4>
	 * <ul>
	 *  <li>no_linked_System</li>
	 * </ul>
	 * <h4>Inventory</h4>
	 * <ul>
	 *  <li>not_enough_inventory_space</li>
	 * </ul>
	 */
	int ReturnCodes_register(lua_State *lua)
	{
		luaL_Reg regs[] = 
		{
			{ "message", ReturnCodes_message },
			{ nullptr, nullptr }
		};

		lua_pushstring(lua, "code");
		lua_newtable(lua);

		luaL_setfuncs(lua, regs, 0);

		LuaState L(lua);
		// General
		L.setTableValue("success", am::base::SUCCESS);
		L.setTableValue("nil_parameter", am::base::NULL_PARAMETER);
		L.setTableValue("internal_error", am::base::INTERNAL_ERROR);
		L.setTableValue("file_not_found", am::base::FILE_NOT_FOUND);
		// Character
		L.setTableValue("able_to_equip", am::base::ABLE_TO_EQUIP);
		L.setTableValue("can_equip", am::base::CAN_EQUIP);
		L.setTableValue("body_part_type_mismatch", am::base::BODY_PART_TYPE_MISMATCH);
		L.setTableValue("not_enough_body_parts", am::base::NOT_ENOUGH_BODY_PARTS);
		L.setTableValue("out_of_range", am::base::OUT_OF_RANGE);
		L.setTableValue("no_weapon_part", am::base::NO_WEAPON_PART);
		L.setTableValue("off_the_map", am::base::OFF_THE_MAP);
		L.setTableValue("invalid_location", am::base::INVALID_LOCATION);
		// Body Parts
		L.setTableValue("body_part_found", am::base::BODY_PART_FOUND);
		L.setTableValue("body_part_not_found", am::base::BODY_PART_NOT_FOUND);
		L.setTableValue("body_part_type_unknown", am::base::BODY_PART_TYPE_UNKNOWN);
		// Texture
		L.setTableValue("texture_load_failed", am::base::TEXTURE_LOAD_FAILED);
		// System
		L.setTableValue("no_linked_system", am::base::NO_LINKED_SYSTEM);
		// Inventory
		L.setTableValue("not_enough_inventory_space", am::base::NOT_ENOUGH_INVENTORY_SPACE);

		lua_settable(lua, -3);

		return 0;
	}

	/**
	 * @static
	 * Accepts a return code value (represented by an integer) and returns
	 * a more detailed message about that return code.
	 * 
	 * @param am.code return_code The return code to get the message for.
	 * @returns string The message associated with the given return code.
	 */
	inline int ReturnCodes_message(lua_State *lua)
	{
		if (lua_isnum(lua, 1))
		{
			if (lua_gettop(lua) == 1)
			{
				lua_pushstring(lua, getErrorMessage(static_cast<am::base::ReturnCode>(lua_tointeger(lua, 1))));
				return 1;
			}
			else if (lua_isstr(lua, 2))
			{
				// Add a new error message.
				// Maybe
			}
		}
		return LuaState::expectedArgs(lua, "message", "am.code code_value");
	}

}
}
