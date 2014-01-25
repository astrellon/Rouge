#pragma once

extern "C" 
{
#	include <lua/src/lstate.h>
} 

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
	bool removeEventListener(lua_State *lua, am::ui::EventInterface *inter, int typen = 2, int funcn = 3, int contextn = 4);

}
}
}
