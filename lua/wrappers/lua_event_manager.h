#pragma once

extern "C" 
{
#	include <lua/src/lstate.h>
}

#include <lua/lua_state.h>
#include <ui/lua_event_listener.h>

namespace am {
namespace ui {
	class EventInterface;
}

namespace lua {
namespace ui {

    int EventInterface_add_event_listener(lua_State *lua, am::ui::EventInterface *inter, int typen = 2, int funcn = 3, int contextn = 4);
    int EventInterface_remove_event_listener(lua_State *lua, am::ui::EventInterface *inter, int typen = 2, int funcn = 3, int contextn = 4);
    int EventInterface_has_event_listener(lua_State *lua, am::ui::EventInterface *inter, int typen = 2);
    

	bool addEventListener(lua_State *lua, am::ui::EventInterface *inter, int typen = 2, int funcn = 3, int contextn = 4);
    template <class T>
    bool addEventListener(lua_State *lua, T *manager, const char *type, int funcRef)
    {
        if (type == nullptr)
        {
            return false;
        }

        wrapRefObject<T>(lua, manager);
    	int contextRef = luaL_ref(lua, LUA_REGISTRYINDEX);
		
		am::ui::LuaEventListener *listener = new am::ui::LuaEventListener(lua, funcRef, contextRef);
		manager->addEventListener(type, listener);
		return true;
    }
	bool removeEventListener(lua_State *lua, am::ui::EventInterface *inter, int typen = 2, int funcn = 3, int contextn = 4);

}
}
}
