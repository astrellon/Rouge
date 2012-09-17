#pragma once

#include <ui/ievent_listener.h>

extern "C"
{
#	include <lua/src/lstate.h>
}

#include <lua/lua_state.h>
using namespace am::lua;

namespace am {
namespace ui {

	class LuaEventListener : public IEventListener {
	public:
		LuaEventListener(lua_State *lua);
		~LuaEventListener();

		virtual lua_State *getLua();

		virtual void onEvent(Event *e);
		virtual void onEvent(MouseEvent *e);
		virtual void onEvent(KeyboardEvent *e);
		virtual void onEvent(DataEvent *e);
		virtual void onEvent(InventoryEvent *e);
		virtual void onEvent(EquipEvent *e);

	protected:

		LuaState mLua;

	};

}
}
