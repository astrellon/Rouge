#include "lua_map_region.h"

extern "C" 
{ 
#	include <lua/src/lua.h>
#	include <lua/src/lauxlib.h>
#	include <lua/src/lualib.h>
}

#include <lua/lua_state.h>
using namespace am::lua;

#include "lua_game.h"

#include <game/map_region.h>
using namespace am::game;

namespace am {
namespace lua {
namespace game {
	/**
	 * @class
	 * Stores a representation of a map region.
	 */
	/**
	 * Creates a new map region instance.
	 *
	 * @param integer width Width of the region.
	 * @param integer height Height of the region.
	 * @param integer [0] initial_value The initial value of each block in the region.
	 */
	int MapRegion_ctor(lua_State *lua)
	{
		int args = lua_gettop(lua);
		if (lua_isnum(lua, 1) && lua_isnum(lua, 2) && (args == 2 ||(args > 2 && lua_isnum(lua, 3))))
		{
			MapRegion *region = nullptr;
			if (args == 2)
			{
				region = new MapRegion(lua_tointeger(lua, 1), lua_tointeger(lua, 2));
			}
			else
			{
				region = new MapRegion(lua_tointeger(lua, 1), lua_tointeger(lua, 2), lua_tointeger(lua, 3));
			}
			wrapRefObject<MapRegion>(lua, region);
			return 1;
		}
		return LuaState::expectedArgs(lua, "@new", "string partName, string [\"unknown_part\"] type");
	}
	/**
	 * Releases body part reference counter.
	 */
	int MapRegion_dtor(lua_State *lua)
	{
		MapRegion *part = castUData<MapRegion>(lua, 1);
		if (part)
		{
			part->release();
			return 0;
		}
		return LuaState::expectedContext(lua, "__gc", "am.map_region");
	}
	/**
	 * Compares if two body parts are the same object.
	 * @param am.map_region rhs The body part to compare with.
	 * @returns boolean True if they are the same object.
	 */
	int MapRegion_eq(lua_State *lua)
	{
		MapRegion *lhs = castUData<MapRegion>(lua, 1);
		if (!lhs)
		{
			return LuaState::expectedContext(lua, "__eq", "am.map_region");
		}
		MapRegion *rhs = castUData<MapRegion>(lua, 2);
		lua_pushboolean(lua, lhs == rhs);
		return 1;
	}

	int MapRegion_register(lua_State *lua)
	{
		luaL_Reg regs[] = 
		{
			{ "new", MapRegion_ctor },
			{ "__gc",  MapRegion_dtor },
			{ "__eq", MapRegion_eq },
			{ "size", MapRegion_size },
			{ "data", MapRegion_data },
			{ "location", MapRegion_location },
			{ "intersects_with", MapRegion_intersects_with },
			{ nullptr, nullptr }
		};

		luaL_newmetatable(lua, MapRegion::LUA_TABLENAME);
		luaL_setfuncs(lua, regs, 0);

		lua_pushvalue(lua, -1);
		lua_setfield(lua, -1, "__index");

		return 1;
	}

	/**
	 * Returns the width and height of the map region.
	 *
	 * @returns integer The width of the region.
	 * @returns integer The height of the region.
	 */
	/**
	 * Sets the width and height of the map region.
	 * Existing map region values are maintained if the new size is larger.
	 *
	 * @param integer width The new width.
	 * @param integer height The new height.
	 * @returns am.map_region This.
	 */
	int MapRegion_size(lua_State *lua)
	{
		MapRegion *region = castUData<MapRegion>(lua, 1);
		if (region)
		{
			int args = lua_gettop(lua);
			if (args == 1)
			{
				lua_pushinteger(lua, region->getWidth());
				lua_pushinteger(lua, region->getHeight());
				return 2;
			}
			else if (lua_isnum(lua, 2) && lua_isnum(lua, 3) && (args == 2 || (args >= 3 && lua_isnum(lua, 4))))
			{
				int width = lua_tointeger(lua, 2);
				int height = lua_tointeger(lua, 3);
				if (args == 2)
				{
					region->setSize(width, height);
				}
				else
				{
					region->setSize(width, height, lua_tointeger(lua, 4));
				}
				lua_first(lua);
			}
			return LuaState::expectedArgs(lua, "size", "integer width, integer height, integer [0] fill_value");
		}
		return LuaState::expectedContext(lua, "size", "am.map_region");
	}

	/**
	 * Returns the map region value at the given location.
	 *
	 * @param integer x The x location.
	 * @param integer y The y location.
	 * @returns integer The map region value.
	 */
	/**
	 * Returns all the map region values in a flat array table.
	 *
	 * @returns table All the map region values.
	 */
	/**
	 * Sets the map region value at the given location.
	 *
	 * @param integer x The x location.
	 * @param integer y The y location.
	 * @param integer value The value to set.
	 * @returns am.map_region This.
	 */
	/**
	 * Sets the map region data from an array table.
	 * The given array table does not have to fit into the map region,
	 * excess values are ignored. And locations past the end of the
	 * given data are left unchanged.
	 *
	 * @param table values The values to set.
	 * @returns am.map_region This.
	 */
	int MapRegion_data(lua_State *lua)
	{
		MapRegion *region = castUData<MapRegion>(lua, 1);
		if (region)
		{
			int args = lua_gettop(lua);
			if (args == 1)
			{
				// Returns all
				int pos = 0;
				int total = region->getWidth() * region->getHeight();
				lua_createtable(lua, total, 0);
				int *data = region->getData();

				while (pos < total)
				{
					lua_pushinteger(lua, pos + 1);
					lua_pushinteger(lua, data[pos]);
					lua_settable(lua, -3);
				}
				return 1;
			}
			else if (lua_istable(lua, 2))
			{
				// Sets all
				lua_pushnil(lua);
				int pos = 0;
				int total = region->getWidth() * region->getHeight();
				int *data = region->getData();
				while(lua_next(lua, -2) && pos < total)
				{
					data[pos] = lua_tointeger(lua, -1);
					pos++;
					lua_pop(lua, 1);
				}
				lua_first(lua);
			}
			else if (lua_isnum(lua, 2) && lua_isnum(lua, 3))
			{
				int x = lua_tointeger(lua, 2);
				int y = lua_tointeger(lua, 3);
				if (lua_isnum(lua, 4))
				{
					// Sets x, y value
					region->setData(x, y, lua_tointeger(lua, 4));
					lua_first(lua);
				}
				else
				{
					// Gets x, y value
					int value = 0;
					region->getData(x, y, value);
					lua_pushinteger(lua, value);
					return 1;
				}
			}
			return LuaState::expectedArgs(lua, "data", 3, "integer x, integer y", "integer x, integer y, integer value", "table values");
		}
		return LuaState::expectedContext(lua, "data", "am.map_region");
	}

	/**
	 * Returns the x and y location of the map region.
	 *
	 * @returns integer The x location.
	 * @returns integer The y location.
	 */
	/**
	 * Sets the x and y location of the map region.
	 *
	 * @param integer x The x location.
	 * @param integer y The y location.
	 * @returns am.map_region This.
	 */
	int MapRegion_location(lua_State *lua)
	{
		MapRegion *region = castUData<MapRegion>(lua, 1);
		if (region)
		{
			int args = lua_gettop(lua);
			if (args == 1)
			{
				Vector2i location = region->getLocation();
				lua_pushinteger(lua, location.x);
				lua_pushinteger(lua, location.y);
				return 2;
			}
			else if (lua_isnum(lua, 2) && lua_isnum(lua, 3))
			{
				region->setLocation(lua_tointeger(lua, 2), lua_tointeger(lua, 3));
				lua_first(lua);
			}
			return LuaState::expectedArgs(lua, "location", "integer x, integer y");
		}
		return LuaState::expectedContext(lua, "location", "am.map_region");
	}

	/**
	 * Returns true if the given game object intersects with
	 * this map region. This will be true if any part of the game object
	 * is over a map region value > 0.
	 *
	 * @param am.game_object object Any game object (am.character, am.item, am.door).
	 * @returns boolean True if it intersects.
	 */
	int MapRegion_intersects_with(lua_State *lua)
	{
		MapRegion *region = castUData<MapRegion>(lua, 1);
		if (region)
		{
			GameObject *obj = getGameObject(lua, 2);
			if (obj)
			{
				lua_pushboolean(lua, region->intersectsWith(obj));
				return 1;
			}
			return LuaState::expectedArgs(lua, "intersects_with", "am.game_object object");
		}
		return LuaState::expectedContext(lua, "intersects_with", "am.map_region");
	}
}
}
}
