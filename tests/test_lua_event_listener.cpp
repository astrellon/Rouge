#include "test_lua_event_listener.h"

#include <tests/asserts.h>

#include <base/handle.h>
using namespace am::base;

#include <lua/lua_state.h>
using namespace am::lua;

#include <lua/wrappers/lua_event_manager.h>
using namespace am::lua::ui;

#include <ui/lua_event_listener.h>
#include <ui/event_manager.h>
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
		EventManager manager;
		LuaState lua;

		int loadResult = lua.loadString("import(\"EventManager\")\n"
			"eventCalled = \"none\"\n"
			"timesCalled = 0\n"
			"theManager = nil\n"
			"local listener = {name=\"Alan\"}\n"
			"listener.eventListen = function(context, event)\n"
			"	eventCalled = event.type\n"
			"	timesCalled = timesCalled + 1\n"
			"	theManager:removeEventListener(\"testEvent\", listener.eventListen, listener)\n"
			"end\n"
			"function setManager(manager)\n"
			"	manager:addEventListener(\"testEvent\", listener.eventListen, listener)\n"
			"	theManager = manager\n"
			"end\n");

		if (!loadResult)
		{
			lua.logStack("LOAD ERR");
		}
		assert(loadResult);

		string eventCalled = lua.getGlobalString("eventCalled");
		assert(eventCalled.compare("none") == 0);
		equals(0, lua.getGlobalInt("timesCalled"));

		assert(lua.hasGlobalFunction("setManager"));
		EventManager_wrap(lua, &manager);

		lua.call(1, 0);
		Handle<Event> testEvent(new Event("testEvent"));
		manager.fireEvent<Event>(testEvent);

		eventCalled = lua.getGlobalString("eventCalled");
		assert(eventCalled.compare("testEvent") == 0);
		equals(1, lua.getGlobalInt("timesCalled"));

		Handle<Event> testEvent2(new Event("testEvent"));
		manager.fireEvent<Event>(testEvent2);

		eventCalled = lua.getGlobalString("eventCalled");
		assert(eventCalled.compare("testEvent") == 0);
		equals(1, lua.getGlobalInt("timesCalled"));
		
		/*EventManager manager;
		LuaState lua;
		LuaEventListener listener(lua);

		assert(lua.loadString("eventCalled = \"none\"\n"
			"function onEvent(event)\n"
			"	eventCalled = event.type\n"
			"end"));

		lua_getglobal(lua, "eventCalled");
		const char *eventCalled = lua_tostring(lua, -1);
		assert(strcmp(eventCalled, "none") == 0);
		lua.pop(1);

		manager.addEventListener("test", &listener);
		Handle<Event> e(new Event("test"));
		manager.fireEvent<Event>(e);

		lua_getglobal(lua, "eventCalled");
		eventCalled = lua_tostring(lua, -1);
		assert(strcmp(eventCalled, "test") == 0);
		lua.pop(1);*/

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
		assert(strcmp("none", eventCalled) == 0);
		equals(-1, lua.getGlobalInt("mouseX"));
		equals(-1, lua.getGlobalInt("mouseY"));
		equals(-1, lua.getGlobalInt("mouseButton"));
		equals(-1, lua.getGlobalInt("localX"));
		equals(-1, lua.getGlobalInt("localY"));

		Handle<MouseEvent> e(new MouseEvent(MOUSE_MOVE, LEFT_BUTTON, 8, 10));
		manager.fireEvent<MouseEvent>(e);

		lua_getglobal(lua, "eventCalled");
		eventCalled = lua_tostring(lua, -1);
		assert(strcmp("mouse_move", eventCalled) == 0);
		equals(8, lua.getGlobalInt("mouseX"));
		equals(10, lua.getGlobalInt("mouseY"));
		equals(1, lua.getGlobalInt("mouseButton"));
		equals(8, lua.getGlobalInt("localX"));
		equals(10, lua.getGlobalInt("localY"));
		*/
		return true;
	}
}
}