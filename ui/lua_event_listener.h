#pragma once

#include <ui/ievent_listener.h>

extern "C"
{
#	include <lua/src/lstate.h>
}

#include <lua/lua_state.h>

namespace am {
namespace ui {

	class LuaEventListener : public IEventListener 
	{
	public:
		LuaEventListener(lua_State *lua, int funcRef, int contextRef = LUA_REFNIL);
		~LuaEventListener();

		virtual lua::LuaState &getLua();

		virtual int getFuncRef() const;
		virtual int getContextRef() const;

		virtual void onEvent(Event *e);
		virtual void onEvent(MouseEvent *e);
		virtual void onEvent(KeyboardEvent *e);
		virtual void onEvent(InventoryEvent *e);
		virtual void onEvent(EquipEvent *e);
		virtual void onEvent(DialogueEvent *e);
		virtual void onEvent(ItemEvent *e);
		virtual void onEvent(MapRegionEvent *e);

		bool operator==(const LuaEventListener *rhs) const;
		bool operator==(const IEventListener *rhs) const;

		virtual bool compareListeners(const LuaEventListener *rhs) const;
		virtual bool compareListeners(const IEventListener *rhs) const;

	protected:

		lua::LuaState mLua;
		int mFuncRef;
		int mContextRef;

	};

}
}
