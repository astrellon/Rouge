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
	 * Creates a new empty CoinPurse.
	 */
	int CoinPurse_ctor(lua_State *lua)
	{
		CoinPurse *purse = new CoinPurse();
		wrapRefObject<CoinPurse>(lua, purse);
		return 1;
	}
	/**
	 * Release the reference count on this CoinPurse.
	 */
	int CoinPurse_dtor(lua_State *lua)
	{
		CoinPurse *purse = castUData<CoinPurse>(lua, 1);
		if (purse)
		{
			purse->release();
		}
		return 0;
	}
	/**
	 * Compares CoinPurses at the reference level.
	 *
	 * @param CoinPurse rhs The other CoinPurse to compare with.
	 * @returns Boolean True if they are the same CoinPurse entity.
	 */
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
			{ "coin", CoinPurse_coin },
			{ "can_add_coin", CoinPurse_can_add_coin },
			{ "can_remove_coin", CoinPurse_can_remove_coin },
			{ "add_coin", CoinPurse_add_coin },
			{ "remove_coin", CoinPurse_remove_coin },
			{ "max_coin", CoinPurse_max_coin },
			{ NULL, NULL }
		};

		luaL_newmetatable(lua, CoinPurse::LUA_TABLENAME);
		luaL_setfuncs(lua, regs, 0);

		lua_pushvalue(lua, -1);
		lua_setfield(lua, -1, "__index");
		
		return 1;
	}
	/**
	 * Returns the amount of coin in this coin purse.
	 * @returns Integer The amount of coin.
	 */
	/**
	 * Sets the amount of coin in this coin purse.
	 * @param Integer coin The amount of coin to set in this coin purse.
	 * @returns CoinPurse This
	 */
	int CoinPurse_coin(lua_State *lua)
	{
		CoinPurse *purse = castUData<CoinPurse>(lua, 1);
		if (purse)
		{
			if (lua_gettop(lua) == 1)
			{
				lua_pushinteger(lua, static_cast<int>(purse->getCoin()));
				return 1;
			}
			else if (lua_isnumber(lua, -1))
			{
				purse->setCoin(static_cast<unsigned int>(lua_tointeger(lua, -1)));
				lua_pushvalue(lua, 1);
				return 1;
			}
		}
		lua_pushnil(lua);
		return 1;
	}
	
	/**
	 * Returns how many coins will not fit if the given amount were to be
	 * added. A result of 0 means that all coins will fit.
	 * @param Integer coin The amount of coin to test.
	 * @returns Integer The amount of coins that won't fit, 0 if all coins will fit.
	 */
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
	/**
	 * Returns how many coins cannot be taken out of the purse. A value of
	 * of zero means that all requested coins can be removed.
	 * @param Integer coin The number of coins to attempt to take out.
	 * @returns Integer The number of coins short.
	 */
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

	/**
	 * Adds the given number of coins to the purse. If the amount of coin
	 * won't fit they will be lost. Use can_add_coin to make sure this doesn't happen.
	 * @param Integer coin The amount of coin to add to the purse.
	 * @returns CoinPurse This
	 */
	int CoinPurse_add_coin(lua_State *lua)
	{
		CoinPurse *purse = castUData<CoinPurse>(lua, 1);
		if (purse && lua_isnumber(lua, -1))
		{
			purse->addCoin(static_cast<unsigned int>(lua_tointeger(lua, -1)));
			lua_pushvalue(lua, 1);
			return 1;
		}
		lua_pushnil(lua);
		return 1;
	}
	/**
	 * Removes the given number of coins from the purse. If the purse does not
	 * have the number of coins then the purse will not go into negative.
	 * @param Integer coin The amount of coin to remove from the purse.
	 * @returns CoinPurse This
	 */
	int CoinPurse_remove_coin(lua_State *lua)
	{
		CoinPurse *purse = castUData<CoinPurse>(lua, 1);
		if (purse && lua_isnumber(lua, -1))
		{
			purse->removeCoin(static_cast<unsigned int>(lua_tointeger(lua, -1)));
		}
		return 0;
	}

	/** 
	 * Returns the maximum number of coins that this purse can hold.
	 * A value of 0 means this purse is unlimited.
	 * @returns Integer The number of coin this purse can fit.
	 */
	/**
	 * Sets the max number of coin this purse can hold.
	 * If the maximum is set to 0, then this purse will have unlimited space.
	 * If the new maximum is lower than the current number of coin
	 * then the additional coins will be lost.
	 * @param Integer max_coin The new max coin amount.
	 * @returns CoinPurse This
	 */
	int CoinPurse_max_coin(lua_State *lua)
	{
		CoinPurse *purse = castUData<CoinPurse>(lua, 1);
		if (purse)
		{
			if (lua_gettop(lua) == 1)
			{
				lua_pushinteger(lua, static_cast<int>(purse->getMaxCoin()));
				return 1;
			}
			else if (lua_isnumber(lua, -1))
			{
				purse->setMaxCoin(static_cast<unsigned int>(lua_tointeger(lua, -1)));
				lua_pushvalue(lua, 1);
				return 1;
			}
		}
		lua_pushnil(lua);
		return 1;
	}

}
}
}
