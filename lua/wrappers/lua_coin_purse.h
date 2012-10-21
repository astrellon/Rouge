#pragma once

extern "C" 
{
#	include <lua/src/lstate.h>
} 

namespace am {
namespace game {
	class CoinPurse;
}

namespace lua {
namespace game {

	const char CoinPurse_tableName[] = "am_game_CoinPurse";

	int CoinPurse_ctor(lua_State *lua);
	void CoinPurse_wrap(lua_State *lua, am::game::CoinPurse *CoinPurse);
	int CoinPurse_dtor(lua_State *lua);
	int CoinPurse_register(lua_State *lua);

	int CoinPurse_set_coin(lua_State *lua);
	int CoinPurse_get_coin(lua_State *lua);

	int CoinPurse_can_add_coin(lua_State *lua);
	int CoinPurse_can_remove_coin(lua_State *lua);

	int CoinPurse_add_coin(lua_State *lua);
	int CoinPurse_remove_coin(lua_State *lua);
	
	am::game::CoinPurse *Check_CoinPurse(lua_State *lua, int n);

}
}
}
