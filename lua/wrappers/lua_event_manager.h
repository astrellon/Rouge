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

	bool addEventListener(lua_State *lua, am::ui::EventInterface *manager);
	bool removeEventListener(lua_State *lua, am::ui::EventInterface *manager);

}
}
}
