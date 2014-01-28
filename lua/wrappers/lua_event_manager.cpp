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

    int EventInterface_add_event_listener(lua_State *lua, am::ui::EventInterface *inter, int typen, int funcn, int contextn)
    {
        if (!inter)
        {
            return 0;
        }

        if (lua_isstr(lua, typen) && lua_isfunction(lua, funcn))
        {
            lua_pushboolean(lua, addEventListener(lua, inter, typen, funcn, contextn));
            return 1;
        }
        return LuaState::expectedArgs(lua, "on", "string event_type, function listener");
    }
    
    int EventInterface_remove_event_listener(lua_State *lua, am::ui::EventInterface *inter, int typen, int funcn, int contextn)
    {
        if (!inter)
        {
            return 0;
        }

        if (lua_isstr(lua, typen) && lua_isfunction(lua, funcn))
        {
            lua_pushboolean(lua, removeEventListener(lua, inter, typen, funcn, contextn));
            return 1;
        }
        return LuaState::expectedArgs(lua, "off", "string event_type, function listener");
    }
    
    int EventInterface_has_event_listener(lua_State *lua, am::ui::EventInterface *inter, int typen)
    {
        if (!inter)
        {
            return 0;
        }

        if (lua_isstr(lua, typen))
        {
            lua_pushboolean(lua, inter->hasEventListener(lua_tostring(lua, typen)));
            return 1;
        }
        return LuaState::expectedArgs(lua, "has_event_listener", "string event_type");
    }

	bool addEventListener(lua_State *lua, am::ui::EventInterface *manager, int typen, int funcn, int contextn)
	{
		const char *eventType = lua_tostring(lua, typen);
		if (eventType == nullptr)
		{
			return false;
		}
		if (!lua_isfunction(lua, funcn))
		{
			return false;
		}
        bool hasContext = contextn != 0 && !lua_isnil(lua, contextn) && lua_istable(lua, contextn);
		/*if (hasContext)
		{
			return false;
		}*/
	
		int contextRef = LUA_REFNIL;
		if (hasContext)
		{
            lua_pushvalue(lua, contextn);
			contextRef = luaL_ref(lua, LUA_REGISTRYINDEX);
		}
        lua_pushvalue(lua, funcn);
		int funcRef = luaL_ref(lua, LUA_REGISTRYINDEX);
		am::ui::LuaEventListener *listener = new am::ui::LuaEventListener(lua, funcRef, contextRef);
		manager->addEventListener(eventType, listener);
		return true;
	}
	bool removeEventListener(lua_State *lua, am::ui::EventInterface *manager, int typen, int funcn, int contextn)
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
        /*bool hasContext = contextn != 0 && !lua_isnil(lua, contextn) && !lua_istable(lua, contextn);
		if (hasContext)
		{
			return false;
		}*/
		bool hasContext = contextn != 0 && !lua_isnil(lua, contextn) && lua_istable(lua, contextn);
		
		int contextRef = LUA_REFNIL;
		if (hasContext)
		{
            lua_pushvalue(lua, contextn);
			contextRef = luaL_ref(lua, LUA_REGISTRYINDEX);
		}
        lua_pushvalue(lua, funcn);
		int funcRef = luaL_ref(lua, LUA_REGISTRYINDEX);
		base::Handle<am::ui::LuaEventListener> listener(new am::ui::LuaEventListener(lua, funcRef, contextRef));
		manager->removeEventListener(eventType, listener);
		return true;
	}

}
}
}
