#include "lua_event_manager.h"

extern "C" 
{ 
#	include <lua/src/lua.h>
#	include <lua/src/lauxlib.h>
#	include <lua/src/lualib.h>
}

#include <lua/lua_state.h>
using namespace am::lua;

#include <ui/event_manager.h>
#include <ui/lua_event_listener.h>
using namespace am::ui;

#include <sstream>
using namespace std;

namespace am {
namespace lua {
namespace ui {

	int EventManager_ctor(lua_State *lua)
	{
		EventManager *manager = new EventManager();
		EventManager_wrap(lua, manager);
		return 1;
	}
	void EventManager_wrap(lua_State *lua, EventManager *manager)
	{
		EventManager ** udata = (EventManager **)lua_newuserdata(lua, sizeof(EventManager *));
		*udata = manager;

		luaL_getmetatable(lua, EventManager_tableName);
		lua_setmetatable(lua, -2);
	}

	int EventManager_dtor(lua_State *lua)
	{
		EventManager *manager = Check_EventManager(lua, 1);
		if (manager)
		{
			delete manager;
		}
		return 0;
	}

	int EventManager_register(lua_State *lua)
	{
		luaL_Reg regs[] = 
		{
			{ "new", EventManager_ctor },
			{ "__gc",  EventManager_dtor },
			{ "addEventListener", EventManager_addEventListener },
			{ "removeEventListener", EventManager_removeEventListener },
			{ "hasEventListener", EventManager_hasEventListener },
			{ NULL, NULL }
		};

		luaL_newmetatable(lua, EventManager_tableName);
		luaL_setfuncs(lua, regs, 0);

		lua_pushvalue(lua, -1);
		lua_setfield(lua, -1, "__index");

		return 1;
	}

	EventManager *Check_EventManager(lua_State *lua, int n)
	{
		return *(EventManager **)luaL_checkudata(lua, n, EventManager_tableName);
	}

	int EventManager_addEventListener(lua_State *lua)
	{
		EventManager *manager = Check_EventManager(lua, 1);
		if (manager)
		{
			const char *eventType = lua_tostring(lua, -3);
			if (eventType == NULL)
			{
				return 0;
			}
			if (!lua_isfunction(lua, -2))
			{
				return 0;
			}
			if (!lua_isnil(lua, -1) && !lua_istable(lua, -1))
			{
				return 0;
			}
			
			int contextRef = luaL_ref(lua, LUA_REGISTRYINDEX);
			int funcRef = luaL_ref(lua, LUA_REGISTRYINDEX);
			LuaEventListener *listener = new LuaEventListener(lua, funcRef, contextRef);
			manager->addEventListener(eventType, listener);
		}
		return 0;
	}
	int EventManager_removeEventListener(lua_State *lua)
	{
		EventManager *manager = Check_EventManager(lua, 1);
		if (manager)
		{
			const char *eventType = lua_tostring(lua, -3);
			if (eventType == NULL)
			{
				return 0;
			}
			if (!lua_isfunction(lua, -2))
			{
				return 0;
			}
			if (!lua_isnil(lua, -1) && !lua_istable(lua, -1))
			{
				return 0;
			}
			
			int contextRef = luaL_ref(lua, LUA_REGISTRYINDEX);
			int funcRef = luaL_ref(lua, LUA_REGISTRYINDEX);
			LuaEventListener *listener = new LuaEventListener(lua, funcRef, contextRef);
			manager->removeEventListener(eventType, listener);
		}
		return 0;
	}
	int EventManager_hasEventListener(lua_State *lua)
	{
		EventManager *manager = Check_EventManager(lua, 1);
		if (manager)
		{
			const char *eventType = lua_tostring(lua, -1);
			if (eventType != NULL)
			{
				lua_pushboolean(lua, manager->hasEventListener(eventType));
				return 1;
			}
		}
		lua_pushboolean(lua, false);
		return 1;
	}

}
}
}
