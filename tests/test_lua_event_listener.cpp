#include "test_lua_event_listener.h"

#include <tests/asserts.h>

#include <base/handle.h>

#include <lua/lua_state.h>
using namespace am::lua;

//#include <lua/wrappers/lua_event_manager.h>
#include <lua/wrappers/game/lua_quest.h>
using namespace am::lua::game;

#include <game/quest.h>
using namespace am::game;

#include <ui/lua_event_listener.h>
#include <ui/event.h>
#include <ui/mouse_event.h>
using namespace am::ui;

extern "C" 
{ 
#	include <lua/src/lua.h>
#	include <lua/src/lauxlib.h>
#	include <lua/src/lualib.h>
} 

namespace am {
namespace tests {

	bool TestLuaEventListener::testSimple() {
		Quest manager("testQuest");
		LuaState lua;

		int loadResult = lua.loadString(
			"eventCalled = \"none\"\n"
			"timesCalled = 0\n"
			"theManager = nil\n"
			"local listener = {name=\"Alan\"}\n"
			"listener.eventListen = function(context, event)\n"
			"	eventCalled = event.type\n"
			"	timesCalled = timesCalled + 1\n"
			"	theManager:off(\"testEvent\", listener.eventListen, listener)\n"
			"end\n"
			"function setManager(manager)\n"
			"	manager:on(\"testEvent\", listener.eventListen, listener)\n"
			"	theManager = manager\n"
			"end\n");
		
		if (!loadResult)
		{
			lua.logStack("LOAD ERR");
		}
		assert(loadResult);

		string eventCalled = lua.getGlobalString("eventCalled");
		am_equalsStr("none", eventCalled.c_str());
		am_equals(0, lua.getGlobalInt("timesCalled"));

		assert(lua.hasGlobalFunction("setManager"));
		wrapRefObject<Quest>(lua, &manager);

		lua_acall(lua, 1, 0);
		am::base::Handle<Event> testEvent(new Event("testEvent"));
		manager.fireEvent<Event>(testEvent);

		eventCalled = lua.getGlobalString("eventCalled");
		am_equalsStr("testEvent", eventCalled.c_str());
		am_equals(1, lua.getGlobalInt("timesCalled"));

		am::base::Handle<Event> testEvent2(new Event("testEvent"));
		manager.fireEvent<Event>(testEvent2);

		eventCalled = lua.getGlobalString("eventCalled");
		am_equalsStr("testEvent", eventCalled.c_str());
		am_equals(1, lua.getGlobalInt("timesCalled"));
		
		return true;
	}

	bool TestLuaEventListener::testMouse()
	{
		/*EventManager manager;
		LuaState lua;
		LuaEventListener listener(lua);
		manager.addEventListener(MOUSE_MOVE, &listener);

		assert(lua.loadString("eventCalled = \"none\"\n"
			"mouseX = -1\n"
			"mouseY = -1\n"
			"mouseButton = -1\n"
			"localX = -1\n"
			"localY = -1\n"
			"function onMouseEvent(event)\n"
			"	eventCalled = event.type\n"
			"	mouseX = event.mouseX\n"
			"	mouseY = event.mouseY\n"
			"	mouseButton = event.mouseButton\n"
			"	localX = event.localX\n"
			"	localY = event.localY\n"
			"end"));

		lua_getglobal(lua, "eventCalled");
		const char *eventCalled = lua_tostring(lua, -1);
		am_equalsStr("none", eventCalled) == 0);
		am_equals(-1, lua.getGlobalInt("mouseX"));
		am_equals(-1, lua.getGlobalInt("mouseY"));
		am_equals(-1, lua.getGlobalInt("mouseButton"));
		am_equals(-1, lua.getGlobalInt("localX"));
		am_equals(-1, lua.getGlobalInt("localY"));

		am::base::Handle<MouseEvent> e(new MouseEvent(MOUSE_MOVE, LEFT_BUTTON, 8, 10));
		manager.fireEvent<MouseEvent>(e);

		lua_getglobal(lua, "eventCalled");
		eventCalled = lua_tostring(lua, -1);
		am_equalsStr("mouse_move", eventCalled) == 0);
		am_equals(8, lua.getGlobalInt("mouseX"));
		am_equals(10, lua.getGlobalInt("mouseY"));
		am_equals(1, lua.getGlobalInt("mouseButton"));
		am_equals(8, lua.getGlobalInt("localX"));
		am_equals(10, lua.getGlobalInt("localY"));
		*/
		return true;
	}
}
}
