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
     *  <li>did_interact</li>
     *  <li>did_not_interact</li>
     *  <li>do_not_interact</li>
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
	 *  <li>item_not_in_inventory</li>
	 * </ul>
	 * <h4>Store</h4>
	 * <ul>
	 *  <li>store_already_owned</li>
	 *  <li>not_enough_coin</li>
	 *  <li>no_store_owner</li>
	 *  <li>no_inventories</li>
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
		L.setTableValue("success", base::SUCCESS);
		L.setTableValue("nil_parameter", base::NULL_PARAMETER);
		L.setTableValue("internal_error", base::INTERNAL_ERROR);
		L.setTableValue("file_not_found", base::FILE_NOT_FOUND);
		// Character
		L.setTableValue("able_to_equip", base::ABLE_TO_EQUIP);
		L.setTableValue("can_equip", base::CAN_EQUIP);
		L.setTableValue("body_part_type_mismatch", base::BODY_PART_TYPE_MISMATCH);
		L.setTableValue("not_enough_body_parts", base::NOT_ENOUGH_BODY_PARTS);
		L.setTableValue("out_of_range", base::OUT_OF_RANGE);
		L.setTableValue("no_weapon_part", base::NO_WEAPON_PART);
		L.setTableValue("off_the_map", base::OFF_THE_MAP);
		L.setTableValue("invalid_location", base::INVALID_LOCATION);
		L.setTableValue("did_interact", base::DID_INTERACT);
		L.setTableValue("did_not_interact", base::DID_NOT_INTERACT);
		L.setTableValue("do_not_interact", base::DO_NOT_INTERACT);
		// Body Parts
		L.setTableValue("body_part_found", base::BODY_PART_FOUND);
		L.setTableValue("body_part_not_found", base::BODY_PART_NOT_FOUND);
		L.setTableValue("body_part_type_unknown", base::BODY_PART_TYPE_UNKNOWN);
		// Texture
		L.setTableValue("texture_load_failed", base::TEXTURE_LOAD_FAILED);
		// System
		L.setTableValue("no_linked_system", base::NO_LINKED_SYSTEM);
		// Inventory
		L.setTableValue("not_enough_inventory_space", base::NOT_ENOUGH_INVENTORY_SPACE);
		L.setTableValue("item_not_in_inventory", base::ITEM_NOT_IN_INVENTORY);
		// Store
		L.setTableValue("store_already_owned", base::STORE_ALREADY_OWNED);
		L.setTableValue("not_enough_coin", base::NOT_ENOUGH_COIN);
		L.setTableValue("no_store_owner", base::NO_STORE_OWNER);
		L.setTableValue("no_inventories", base::NO_INVENTORIES);

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
				lua_pushstring(lua, getErrorMessage(static_cast<base::ReturnCode>(lua_tointeger(lua, 1))));
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
