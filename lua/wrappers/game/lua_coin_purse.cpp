#include "lua_coin_purse.h"

extern "C" 
{ 
#	include <lua/src/lua.h>
#	include <lua/src/lauxlib.h>
#	include <lua/src/lualib.h>
}

#include <lua/lua_state.h>
using namespace am::lua;

#include <game/coin_purse.h>
using namespace am::game;

namespace am {
namespace lua {
namespace game {

	/**
	 * Creates a new CoinPurse
	 */
	int CoinPurse_ctor(lua_State *lua)
	{
		CoinPurse *purse = new CoinPurse();
		wrapRefObject<CoinPurse>(lua, purse);
		return 1;
	}

	int CoinPurse_dtor(lua_State *lua)
	{
		CoinPurse *purse = castUData<CoinPurse>(lua, 1);
		if (purse)
		{
			purse->release();
		}
		return 0;
	}

	int CoinPurse_eq(lua_State *lua)
	{
		CoinPurse *lhs = castUData<CoinPurse>(lua, 1);
		CoinPurse *rhs = castUData<CoinPurse>(lua, 2);
		lua_pushboolean(lua, lhs == rhs);
		return 1;
	}

	int CoinPurse_register(lua_State *lua)
	{
		luaL_Reg regs[] = 
		{
			{ "new", CoinPurse_ctor },
			{ "__gc", CoinPurse_dtor },
			{ "__eq", CoinPurse_eq },
			{ "set_coin", CoinPurse_set_coin },
			{ "get_coin", CoinPurse_get_coin },
			{ "can_add_coin", CoinPurse_can_add_coin },
			{ "can_remove_coin", CoinPurse_can_remove_coin },
			{ "add_coin", CoinPurse_add_coin },
			{ "remove_coin", CoinPurse_remove_coin },
			{ "set_max_coin", CoinPurse_set_max_coin },
			{ "get_max_coin", CoinPurse_get_max_coin },
			{ NULL, NULL }
		};

		luaL_newmetatable(lua, CoinPurse::LUA_TABLENAME);
		luaL_setfuncs(lua, regs, 0);

		lua_pushvalue(lua, -1);
		lua_setfield(lua, -1, "__index");
		
		return 1;
	}

	/*CoinPurse *castUData<CoinPurse>(lua_State *lua, int n)
	{
		return *(CoinPurse **)luaL_checkudata(lua, n, CoinPurse_tableName);
	}*/

	int CoinPurse_set_coin(lua_State *lua)
	{
		CoinPurse *purse = castUData<CoinPurse>(lua, 1);
		if (purse && lua_isnumber(lua, -1))
		{
			purse->setCoin(static_cast<unsigned int>(lua_tointeger(lua, -1)));
		}
		return 0;
	}
	int CoinPurse_get_coin(lua_State *lua)
	{
		CoinPurse *purse = castUData<CoinPurse>(lua, 1);
		if (purse)
		{
			lua_pushinteger(lua, static_cast<int>(purse->getCoin()));
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}

	int CoinPurse_can_add_coin(lua_State *lua)
	{
		CoinPurse *purse = castUData<CoinPurse>(lua, 1);
		if (purse && lua_isnumber(lua, -1))
		{
			lua_pushinteger(lua, static_cast<int>(purse->canAddCoin(lua_tointeger(lua, -1))));
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}
	int CoinPurse_can_remove_coin(lua_State *lua)
	{
		CoinPurse *purse = castUData<CoinPurse>(lua, 1);
		if (purse && lua_isnumber(lua, -1))
		{
			lua_pushinteger(lua, static_cast<int>(purse->canRemoveCoin(lua_tointeger(lua, -1))));
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}

	int CoinPurse_add_coin(lua_State *lua)
	{
		CoinPurse *purse = castUData<CoinPurse>(lua, 1);
		if (purse && lua_isnumber(lua, -1))
		{
			purse->addCoin(static_cast<unsigned int>(lua_tointeger(lua, -1)));
		}
		return 0;
	}
	int CoinPurse_remove_coin(lua_State *lua)
	{
		CoinPurse *purse = castUData<CoinPurse>(lua, 1);
		if (purse && lua_isnumber(lua, -1))
		{
			purse->removeCoin(static_cast<unsigned int>(lua_tointeger(lua, -1)));
		}
		return 0;
	}

	int CoinPurse_set_max_coin(lua_State *lua)
	{
		CoinPurse *purse = castUData<CoinPurse>(lua, 1);
		if (purse && lua_isnumber(lua, -1))
		{
			purse->setMaxCoin(static_cast<unsigned int>(lua_tointeger(lua, -1)));
		}
		return 0;
	}
	int CoinPurse_get_max_coin(lua_State *lua)
	{
		CoinPurse *purse = castUData<CoinPurse>(lua, 1);
		if (purse)
		{
			lua_pushinteger(lua, static_cast<int>(purse->getMaxCoin()));
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}

}
}
}
