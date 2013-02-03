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

	int CoinPurse_ctor(lua_State *lua);
	int CoinPurse_dtor(lua_State *lua);
	int CoinPurse_eq(lua_State *lua);
	int CoinPurse_register(lua_State *lua);

	int CoinPurse_coin(lua_State *lua);

	int CoinPurse_can_add_coin(lua_State *lua);
	int CoinPurse_can_remove_coin(lua_State *lua);

	int CoinPurse_add_coin(lua_State *lua);
	int CoinPurse_remove_coin(lua_State *lua);

	int CoinPurse_max_coin(lua_State *lua);

}
}
}
