#include <ui/lua_event_listener.h>

#include <lua/wrappers/game/lua_item.h>
#include <lua/wrappers/game/lua_inventory.h>
#include <lua/wrappers/game/lua_character.h>
#include <lua/wrappers/game/lua_dialogue.h>
using namespace am::lua::game;

#include <sstream>
using namespace std;

#include <game/character.h>
#include <game/inventory.h>
using namespace am::game;

namespace am {
namespace ui {

	LuaEventListener::LuaEventListener(lua_State *lua, int funcRef, int contextRef) :
		mLua(lua),
		mFuncRef(funcRef),
		mContextRef(contextRef)
	{

	}
	LuaEventListener::~LuaEventListener()
	{

	}

	lua_State *LuaEventListener::getLua()
	{
		return mLua.getLua();
	}

	int LuaEventListener::getFuncRef() const
	{
		return mFuncRef;
	}
	int LuaEventListener::getContextRef() const
	{
		return mContextRef;
	}

	void LuaEventListener::onEvent(Event *e)
	{
		lua_rawgeti(mLua, LUA_REGISTRYINDEX, mFuncRef);
		bool contexted = mContextRef != LUA_REFNIL;
		if (contexted)
		{
			lua_rawgeti(mLua, LUA_REGISTRYINDEX, mContextRef);
		}
		mLua.newTable();
		mLua.setTableValue("type", e->getType().c_str());
		//lua_call(mLua, contexted ? 2 : 1, 0);
		lua_pcall(mLua, contexted ? 2 : 1, 0, 0);
	}
	void LuaEventListener::onEvent(MouseEvent *e)
	{
		lua_rawgeti(mLua, LUA_REGISTRYINDEX, mFuncRef);
		bool contexted = mContextRef != LUA_REFNIL;
		if (contexted)
		{
			lua_rawgeti(mLua, LUA_REGISTRYINDEX, mContextRef);
		}
		mLua.newTable();
		mLua.setTableValue("type", e->getType().c_str());
		mLua.setTableValue("localX", e->getLocalMouseX());
		mLua.setTableValue("localY", e->getLocalMouseY());
		mLua.setTableValue("mouseButton", static_cast<int>(e->getMouseButton()));
		mLua.setTableValue("mouseX", e->getMouseX());
		mLua.setTableValue("mouseY", e->getMouseY());
		lua_call(mLua, contexted ? 2 : 1, 0);
	}
	void LuaEventListener::onEvent(KeyboardEvent *e)
	{
		lua_rawgeti(mLua, LUA_REGISTRYINDEX, mFuncRef);
		bool contexted = mContextRef != LUA_REFNIL;
		if (contexted)
		{
			lua_rawgeti(mLua, LUA_REGISTRYINDEX, mContextRef);
		}
		mLua.newTable();
		mLua.setTableValue("type", e->getType().c_str());
		mLua.setTableValue("key", e->getKey());
		mLua.setTableValue("isSystemKey", e->isSystemKey());
		lua_call(mLua, contexted ? 2 : 1, 0);
	}
	void LuaEventListener::onEvent(InventoryEvent *e)
	{
		lua_rawgeti(mLua, LUA_REGISTRYINDEX, mFuncRef);
		bool contexted = mContextRef != LUA_REFNIL;
		if (contexted)
		{
			lua_rawgeti(mLua, LUA_REGISTRYINDEX, mContextRef);
		}
		mLua.newTable();
		mLua.setTableValue("type", e->getType().c_str());
		mLua.setTableValue("spotX", e->getSpotX());
		mLua.setTableValue("spotY", e->getSpotY());
		mLua.push("item");
		wrapRefObject<Item>(mLua, e->getItem());
		lua_settable(mLua, -3);
		mLua.push("inventory");
		wrapRefObject<Inventory>(mLua, e->getInventory());
		lua_settable(mLua, -3);
		lua_call(mLua, contexted ? 2 : 1, 0);
	}
	void LuaEventListener::onEvent(EquipEvent *e)
	{
		lua_rawgeti(mLua, LUA_REGISTRYINDEX, mFuncRef);
		bool contexted = mContextRef != LUA_REFNIL;
		if (contexted)
		{
			lua_rawgeti(mLua, LUA_REGISTRYINDEX, mContextRef);
		}
		mLua.newTable();
		mLua.setTableValue("type", e->getType().c_str());
		mLua.setTableValue("bodyPartName", e->getBodyPartName());
		mLua.push("item");
		wrapRefObject<Item>(mLua, e->getItem());
		lua_settable(mLua, -3);
		mLua.push("character");
		wrapRefObject<Character>(mLua, e->getCharacter());
		lua_settable(mLua, -3);
		lua_call(mLua, contexted ? 2 : 1, 0);
	}
	void LuaEventListener::onEvent(DialogueEvent *e)
	{
		lua_rawgeti(mLua, LUA_REGISTRYINDEX, mFuncRef);
		bool contexted = mContextRef != LUA_REFNIL;
		if (contexted)
		{
			lua_rawgeti(mLua, LUA_REGISTRYINDEX, mContextRef);
		}
		mLua.newTable();
		mLua.setTableValue("type", e->getType().c_str());
		// Talked To
		mLua.push("talkedTo");
		Character *isChar = dynamic_cast<Character *>(e->getTalkedTo());
		if (isChar)
		{
			wrapRefObject<Character>(mLua, isChar);
		}
		else
		{
			lua_pushnil(mLua);
		}
		lua_settable(mLua, -3);
		// Talker
		mLua.push("talker");
		isChar = dynamic_cast<Character *>(e->getTalker());
		if (isChar)
		{
			wrapRefObject<Character>(mLua, isChar);
		}
		else
		{
			lua_pushnil(mLua);
		}
		lua_settable(mLua, -3);
		// Dialogue
		mLua.push("dialogue");
		Dialogue *diag = e->getDialogue();
		if (diag)
		{
			wrapObject<Dialogue>(mLua, diag);
		}
		else
		{
			lua_pushnil(mLua);
		}
		lua_settable(mLua, -3);

		lua_call(mLua, contexted ? 2 : 1, 0);
	}

	bool LuaEventListener::operator==(const LuaEventListener *rhs) const
	{
		return compareListeners(rhs);
	}

	bool LuaEventListener::operator==(const IEventListener *rhs) const
	{
		const LuaEventListener *rhsLua = dynamic_cast<const LuaEventListener *>(rhs);
		if (rhsLua)
		{
			return compareListeners(rhsLua);
		}
		return false;
	}

	bool LuaEventListener::compareListeners(const LuaEventListener *rhs) const
	{
		if (rhs == NULL)
		{
			return false;
		}
		if (rhs->mLua != mLua)
		{
			return false;
		}
		return mLua.compareRefs(mContextRef, rhs->mContextRef) &&
			mLua.compareRefs(mFuncRef, rhs->mFuncRef);
	}

	bool LuaEventListener::compareListeners(const IEventListener *rhs) const
	{
		const LuaEventListener *rhsLua = dynamic_cast<const LuaEventListener *>(rhs);
		if (rhsLua)
		{
			return compareListeners(rhsLua);
		}
		return false;
	}
}
}
