#include "lua_event_manager.h"

extern "C" 
{ 
#	include <lua/src/lua.h>
#	include <lua/src/lauxlib.h>
#	include <lua/src/lualib.h>
}

#include <lua/lua_state.h>

#include <ui/event_interface.h>
#include <ui/lua_event_listener.h>
#include <base/handle.h>

#include <sstream>

namespace am {
namespace lua {
namespace ui {

    int EventInterface_add_event_listener(lua_State *lua, am::ui::EventInterface *inter)
    {
        if (!inter)
        {
            return 0;
        }

        if (lua_isstr(lua, 2) && lua_isfunction(lua, 3))
        {
            lua_pushboolean(lua, addEventListener(lua, inter));
            return 1;
        }
        return LuaState::expectedArgs(lua, "on", "string event_type, function listener");
    }
    
    int EventInterface_remove_event_listener(lua_State *lua, am::ui::EventInterface *inter)
    {
        if (!inter)
        {
            return 0;
        }

        if (lua_isstr(lua, 2) && lua_isfunction(lua, 3))
        {
            lua_pushboolean(lua, removeEventListener(lua, inter));
            return 1;
        }
        return LuaState::expectedArgs(lua, "off", "string event_type, function listener");
    }
    
    int EventInterface_has_event_listener(lua_State *lua, am::ui::EventInterface *inter)
    {
        if (!inter)
        {
            return 0;
        }

        if (lua_isstr(lua, 2))
        {
            lua_pushboolean(lua, inter->hasEventListener(lua_tostring(lua, 2)));
            return 1;
        }
        return LuaState::expectedArgs(lua, "has_event_listener", "string event_type");
    }

	bool addEventListener(lua_State *lua, am::ui::EventInterface *manager)
	{
		const char *eventType = lua_tostring(lua, 2);
		if (eventType == nullptr)
		{
			return false;
		}
		if (!lua_isfunction(lua, 3))
		{
			return false;
		}
		bool hasContext = lua_gettop(lua) >= 4;
		if (hasContext && !lua_isnil(lua, 4) && !lua_istable(lua, 4))
		{
			return false;
		}
		
		int contextRef = LUA_REFNIL;
		if (hasContext)
		{
			contextRef = luaL_ref(lua, LUA_REGISTRYINDEX);
		}
		int funcRef = luaL_ref(lua, LUA_REGISTRYINDEX);
		am::ui::LuaEventListener *listener = new am::ui::LuaEventListener(lua, funcRef, contextRef);
		manager->addEventListener(eventType, listener);
		return true;
	}
	bool removeEventListener(lua_State *lua, am::ui::EventInterface *manager)
	{
		const char *eventType = lua_tostring(lua, 2);
		if (eventType == nullptr)
		{
			return false;
		}
		if (!lua_isfunction(lua, 3))
		{
			return false;
		}
		bool hasContext = lua_gettop(lua) >= 4;
		if (hasContext && !lua_isnil(lua, 4) && !lua_istable(lua, 4))
		{
			return false;
		}
		
		int contextRef = LUA_REFNIL;
		if (hasContext)
		{
			contextRef = luaL_ref(lua, LUA_REGISTRYINDEX);
		}
		int funcRef = luaL_ref(lua, LUA_REGISTRYINDEX);
		base::Handle<am::ui::LuaEventListener> listener(new am::ui::LuaEventListener(lua, funcRef, contextRef));
		manager->removeEventListener(eventType, listener);
		return true;
	}

}
}
}
