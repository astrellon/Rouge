#include "lua_iattribute_data.h"

extern "C" 
{ 
#	include <lua/src/lua.h>
#	include <lua/src/lauxlib.h>
#	include <lua/src/lualib.h>
}

#include <lua/lua_state.h>
using namespace am::lua;

#include <game/iattribute_data.h>
using namespace am::game;

namespace am {
namespace lua {
namespace game {
	
	int IAttributeData_attrs(lua_State *lua, am::game::IAttributeData *attr)
	{
		int args = lua_gettop(lua);
		bool getAttrs = args == 1 || (args == 2 && lua_isbool(lua, 2));
		if (getAttrs)
		{
			bool create = args == 2 && lua_tobool(lua, 2);
			data::Table *table = attr->getAttributes(create);
			if (table)
			{
				wrapRefObject<data::Table>(lua, table);
			}
			else
			{
				lua_pushnil(lua);
			}
			return 1;
		}
		else
		{
			data::Table *table = castUData<data::Table>(lua, 2);
			if (table)
			{
				attr->setAttributes(table);
				lua_first(lua);
			}
			if (lua_isnil(lua, 2))
			{
				attr->setAttributes(NULL);
				lua_first(lua);
			}
		}
		return LuaState::expectedArgs(lua, "attrs", 2, "am.data_table attrs_table", "nil attrs_table");
	}

}
}
}
