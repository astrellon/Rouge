#include <ui/lua_event_listener.h>

namespace am {
namespace ui {

	LuaEventListener::LuaEventListener(lua_State *lua) :
		mLua(lua)
	{

	}
	LuaEventListener::~LuaEventListener()
	{

	}

	lua_State *LuaEventListener::getLua()
	{
		return mLua.getLua();
	}

	void LuaEventListener::onEvent(Event *e)
	{
		if (mLua.hasGlobalFunction("onEvent"))
		{
			mLua.newTable();
			mLua.setTableValue("type", e->getType().c_str());
			lua_call(mLua, 1, 0);
		}
	}
	void LuaEventListener::onEvent(MouseEvent *e)
	{
		if (mLua.hasGlobalFunction("onMouseEvent"))
		{
			mLua.newTable();
			mLua.setTableValue("type", e->getType().c_str());
			mLua.setTableValue("localX", e->getLocalMouseX());
			mLua.setTableValue("localY", e->getLocalMouseY());
			mLua.setTableValue("mouseButton", static_cast<int>(e->getMouseButton()));
			mLua.setTableValue("mouseX", e->getMouseX());
			mLua.setTableValue("mouseY", e->getMouseY());
			lua_call(mLua, 1, 0);
		}
	}
	void LuaEventListener::onEvent(KeyboardEvent *e)
	{
		
	}
	void LuaEventListener::onEvent(DataEvent *e)
	{

	}
	void LuaEventListener::onEvent(InventoryEvent *e)
	{

	}
	void LuaEventListener::onEvent(EquipEvent *e)
	{

	}

}
}
